#include "FileReader.h"
#include <algorithm>

static void ClearQueue(std::queue<STask>& q)
{
	std::queue<STask> empty;
	std::swap(q, empty);
}

void FileReader::WordCounter(const std::string& str)
{
	std::stringstream ss;
	ss << str;

	std::string word;
	while (ss >> word)
	{
		std::lock_guard<std::mutex> lock(map_mutex_);
		words_[word]++;
	}
}

void FileReader::Consume()
{
	STask task;

	while (!task.done_)
	{
		{
			std::unique_lock<std::mutex> lock(q_mutex_);
			while (tasks_.empty())
				cond_.wait(lock);
				//continue; //since we don't work with condition variable simply continue

			task = tasks_.front();

			if (!task.done_)
				tasks_.pop();
		}

		if (!task.done_)
		{
			WordCounter(task.line);
		}
	}
}

void FileReader::Produce(const char* file)
{
	std::ifstream src(file);

	//could grab multiple lines to process for each consumer
	//depending on the line size
	std::string line;
	while (std::getline(src, line))
	{
		//STask task;
		//task.line = line;
		//{
		//	std::lock_guard<std::mutex> lock{ q_mutex_ };
		//	tasks_.push(task);
		//}

		std::lock_guard<std::mutex> lock{ q_mutex_ };
		STask task;
		task.line = line;
		tasks_.push(task);
		cond_.notify_all();
	}

	//Add done to the list, so the consumer sees we are done
	{
		std::lock_guard<std::mutex> lock{ q_mutex_ };
		STask task;
		task.done_ = true;
		tasks_.push(task);
		cond_.notify_all();
	}
}

bool FileReader::Process(const char* file)
{
	if (!std::filesystem::is_regular_file(file))
	{
		std::cerr << file << " isn't a file or available!" << std::endl;
		return false;
	}

	const auto cores = std::thread::hardware_concurrency();
	consumer_.reserve(cores);
	for (auto i = 0; i < cores; i++)
	{
		consumer_.push_back(std::thread(&FileReader::Consume, this));
	}

	Produce(file);

	for (auto& item : consumer_)
	{
		item.join();
	}

	ClearQueue(tasks_);
	consumer_.clear();

	return true;
}

void FileReader::PrintResult()
{
	printf("%-15s %s\n", "Wort", "Anzahl");
	for (auto& [word, count] : words_)
	{
		printf("%-15s %d\n", std::string(word).c_str(), count);
	}
}