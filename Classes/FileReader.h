#pragma once

#include <filesystem>
#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>

#include <unordered_map>
#include <mutex>
#include <thread>



/**
*	Using mutex for simplicity.
*	Thread could be put to sleep when waiting for tasks in a queue with an condition variable.
*	Could be optimized for a lockfree like consume/produce with atomics
*/
class FileReader
{
	struct STask
	{
		std::string line;
		bool done_ = false;
	};

	using WordMap = std::unordered_map<std::string, int>;

public:
	bool Process(const char* file);
	void PrintResult();

private:
	/**
	*	Consumes tasks from the queue.
	*	Used by each thread.
	*/
	void Consume();

	/**
	*	Produces Tasks for the all the consumers.
	*/
	void Produce(const char* file);

	/**
	*	Counts how many times a number occurs in a given string.
	*	@param str String used as source for thr the counter.
	*/
	void WordCounter(const std::string& str);

private:
	WordMap words_;
	std::vector<std::thread> consumer_;
	std::queue<STask> tasks_;

	std::mutex q_mutex_;
	std::mutex map_mutex_;
};