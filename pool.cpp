#include "pool.h"
using namespace std;

Pool::Pool(string strName)
    : name_(strName), threadCount_(0), maxQueueSize_(4), running_(false) {}

void Pool::start(int nThreadNums) {
  threadCount_ = nThreadNums;
  running_ = true;
  for (int i = 0; i < nThreadNums; i++) {
    threads_.push_back(thread(&Pool::runInThread, this));
  }
  cout << __LINE__ << "threads:" << threads_.size() << endl;
}
void Pool::stop() {
  running_ = false;
  for (size_t i = 0; i < threadCount_; i++) {
    threads_[i].join();
  }
}

void Pool::run(task t) {
  cout << __LINE__ << "threads:" << threads_.size() << endl;

  if (threads_.empty()) {
    t();
  } else {
    std::unique_lock<std::mutex> lk_(mutex_);
    full_.wait(lk_, [&]() { return queue_.size() == maxQueueSize_; });
    queue_.push(std::move(t));
  }
}

task Pool::take() {

  std::unique_lock<std::mutex> ulk_(mutex_);

  empty_.wait(ulk_, [this]() { return queue_.size() > 0; });
  std::lock_guard<std::mutex> lk_(mutex_);
  task t = queue_.front();
  queue_.pop();
  ulk_.unlock();
  return t;
}

void Pool::runInThread() {
  if (threadInitCallBack_) {
    threadInitCallBack_();
  }
  cout << __LINE__ << endl;
  while (running_) {
    cout << __LINE__ << endl;

    task t = take();
    cout << __LINE__ << endl;

    if (t) {
      cout << __LINE__ << endl;

      t();
    }
  }
}

Pool::~Pool() { stop(); }
