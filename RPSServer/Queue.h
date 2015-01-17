#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>


// Ref: https://juanchopanzacpp.wordpress.com/2013/02/26/concurrent-queue-c11/
// A thread-safe blockable queue.
namespace nwayinterview
{
	template <typename T>
	class Queue
	{
	public:

		T pop()
		{
			std::unique_lock<std::mutex> mlock(mutex_);
			while (queue_.empty())
			{
				cond_.wait(mlock);
			}
			auto item = queue_.front();
			queue_.pop();
			return item;
		}

		void pop(T& item)
		{
			std::unique_lock<std::mutex> mlock(mutex_);
			while (queue_.empty())
			{
				cond_.wait(mlock);
			}
			item = queue_.front();
			queue_.pop();
		}

		bool try_pop(T& item)
		{
			std::unique_lock<std::mutex> mlock(mutex_);
			if (std::try_lock(mlock) != -1)  // -1 means locked, according to stl doc.
				return false;
			
			if (queue_.empty())
				return false;
			
			item = queue_.front();
			queue_.pop();

			return true;
		}

		void push(const T& item)
		{
			std::unique_lock<std::mutex> mlock(mutex_);
			queue_.push(item);
			mlock.unlock();
			cond_.notify_one();
		}

		void push(T&& item)
		{
			std::unique_lock<std::mutex> mlock(mutex_);
			queue_.push(std::move(item));
			mlock.unlock();
			cond_.notify_one();
		}

	private:
		std::queue<T> queue_;
		std::mutex mutex_;
		std::condition_variable cond_;
	};
}