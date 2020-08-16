#include <condition_variable>
#include <functional>
#include <iostream>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <vector>

typedef std::function<void()> task;

class Pool {
public:
  Pool(std::string strName = "thread pool");
  ~Pool();
  void setMaxQueueSize(int nSize) { maxQueueSize_ = nSize; }
  void setThreadInitCallBack(const task &f) { threadInitCallBack_ = f; }
  void start(int nThreadNums);
  void stop();
  std::string name() const { return name_; }
  int queueSize() const { return maxQueueSize_; }
  void run(task f);

private:
   
  bool isFull() {   std::lock_guard<std::mutex> lk_(mutex_);return threadCount_ == threads_.size(); }
  void runInThread();
  task take();
  std::vector<std::thread> threads_;
  std::queue<task> queue_;
  task threadInitCallBack_;
  std::string name_;
  int threadCount_;
  int maxQueueSize_;
  bool running_;
  std::mutex mutex_;
  std::condition_variable empty_;
  std::condition_variable full_;
};