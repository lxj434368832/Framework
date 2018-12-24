#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <cassert>

// 参考：http://www.cnblogs.com/haippy/p/3284540.html

namespace producer_consumer {

const int kRepositorySize = 10;
const int kDataCount = 100;

struct Repository
{
    int datas[kRepositorySize];
    int read_position;
    int write_position;

    // 这个仓库是共享资源，也就是说消费者生产者读写同一个仓库，所以消费者生产者要用同一个互斥锁来实现线程安全。
    // 所以这个互斥锁放在Repository这个类里面更合适。
    std::mutex mutex;

    // 用条件变量来实现线程同步。每个角色都拥有属于自己执行条件的变量。
    // 对于生产者来说，不为满是执行条件，对于消费者来说不为空是执行条件。
    std::condition_variable not_full_cv;
    std::condition_variable not_empty_cv;

    // 用于多消费者情况。
    int consumed_count;  // 已消费的数量。
    std::mutex consumer_mutex;  // 多消费者之间的互斥量，用于consumed_count的线程安全。

    // 用于多生产者情况。
    int produced_count;  // 已生产的数量。
    std::mutex producer_mutex;  // 多生产者之间的互斥量，用于produced_count的线程安全。

    // 准备生产的数据。
    std::vector<int> planed_datas;

    Repository()
    {
        memset(datas, 0, kRepositorySize);
        consumed_count = 0;
        produced_count = 0;
        read_position = 0;
        write_position = 0;

        for (int i = 0; i < kDataCount; ++i)
        {
            planed_datas.push_back(i + 1);
        }   //end for
    } //end constructor
};

static void Producer(Repository& repository, int data)
{
    std::cout << "+++Thread-" << std::this_thread::get_id() << ": produce: " << data << std::endl;

    std::unique_lock<std::mutex> lock(repository.mutex);

    // 循环队列有一个元素是不放值的，用于判断队列填满的条件。仓库满了则开始等待。
    while ((repository.write_position + 1) % kRepositorySize == repository.read_position)
    {
        repository.not_full_cv.wait(lock);  // wait not full, 等待仓库不为满。
    }

    repository.datas[repository.write_position++] = data;
    repository.write_position %= kRepositorySize;

    // 生产者生产了数据，那么仓库不为空，则可以通知消费者去消费。
    repository.not_empty_cv.notify_all();
}

static void Consumer(Repository& repository)
{
    std::unique_lock<std::mutex> lock(repository.mutex);

    // 循环队列，队列为空的条件。仓库为空则开始等待。
    while (repository.read_position == repository.write_position)
    {
        repository.not_empty_cv.wait(lock);  // wait not empty, 等待仓库不为空。
    }

    int data = repository.datas[repository.read_position++];
    repository.read_position %= kRepositorySize;

    // 消费者数据，那么仓库不为满，则可以通知生产者去生产。
    repository.not_full_cv.notify_all();

    std::cout << "---Thread-" << std::this_thread::get_id() << ": consume: " << data << std::endl;
}

//-------------------------------------------------------------------------------
// 1.单生产者-单消费者。
// 单生产者 - 单消费者模型中只有一个生产者和一个消费者，生产者不停地往产品库中放入产品，
// 消费者则从产品库中取走产品，产品库容积有限制，只能容纳一定数目的产品，如果生产者生产产品的速度过快，
// 则需要等待消费者取走产品之后，产品库不为空才能继续往产品库中放置新的产品，相反，如果消费者取走产品的速度过快，
// 则可能面临产品库中没有产品可使用的情况，此时需要等待生产者放入一个产品后，消费者才能继续工作。

static void StartProduce1(Repository& repository)
{
    for (int i = 0; i < kDataCount; ++i)
    {
        Producer(repository, repository.planed_datas[i]);
    }
}

static void StartConsume1(Repository& repository)
{
    int count = 0;

    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        Consumer(repository);
        ++count;
        if (count >= kDataCount)
        {
            break;
        }
    }
}

static void Test1()
{
    Repository repository;

    std::thread producer(StartProduce1, std::ref(repository));
    std::thread consumer(StartConsume1, std::ref(repository));

    producer.join();
    consumer.join();
}

//--------------------------------------------------------------------------
// 2.单生产者-多消费者。
// 单生产者-多消费者模型中可以允许多个消费者同时从产品库中取走产品。
// 所以除了保护产品库在多个读写线程下互斥之外，还需要维护消费者取走产品的计数器。

static void StartConsume2(Repository& repository)
{
    while (true)
    {
        // 多个消费者之间互斥，保证consumed_count线程安全，否则不能正确知道消费的产品的数量。
        std::unique_lock<std::mutex> lock(repository.consumer_mutex);
        // 已经消费的产品数量大于等于总产品数量时，退出。
        if (repository.consumed_count >= kDataCount)
        {
            break;
        }

        ++repository.consumed_count;
        Consumer(repository);

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

static void Test2()
{
    Repository repository;

    std::thread producer(StartProduce1, std::ref(repository));
    std::thread consumer1(StartConsume2, std::ref(repository));
    std::thread consumer2(StartConsume2, std::ref(repository));
    std::thread consumer3(StartConsume2, std::ref(repository));

    producer.join();
    consumer1.join();
    consumer2.join();
    consumer3.join();
}

//--------------------------------------------------------------------------
// 3.多生产者-单消费者。
// 多生产者-单消费者模型中可以允许多个生产者同时向产品库中放入产品。
// 所以除了保护产品库在多个读写线程下互斥之外，还需要维护生产者放入产品的计数器。

static void StartProduce3(Repository& repository)
{
    while (true)
    {
        // 多个生产者之间互斥，保证produced_count线程安全，否则不能正确知道生产了多少产品。
        std::unique_lock<std::mutex> lock(repository.producer_mutex);
        // 生产的产品数量大于计划的总产品数量时，退出。
        if (repository.produced_count >= kDataCount)
        {
            break;
        }

        Producer(repository, repository.planed_datas[repository.produced_count]);
        ++repository.produced_count;

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

static void Test3() {
    Repository repository;

    std::thread producer1(StartProduce3, std::ref(repository));
    std::thread producer2(StartProduce3, std::ref(repository));
    std::thread producer3(StartProduce3, std::ref(repository));

    std::thread consumer(StartConsume1, std::ref(repository));

    producer1.join();
    producer2.join();
    producer3.join();
    consumer.join();
}

//--------------------------------------------------------------------------
// 4.多生产者-多消费者。
// 该模型可以说是前面两种模型的综合，程序需要维护两个计数器，分别是生产者已生产产品的数目
// 和消费者已取走产品的数目。另外也需要保护产品库在多个生产者和多个消费者互斥地访问。

static void Test4() {
    Repository repository;

    std::thread producer1(StartProduce3, std::ref(repository));
    std::thread producer2(StartProduce3, std::ref(repository));
    std::thread producer3(StartProduce3, std::ref(repository));

    std::thread consumer1(StartConsume2, std::ref(repository));
    std::thread consumer2(StartConsume2, std::ref(repository));
    std::thread consumer3(StartConsume2, std::ref(repository));

    producer1.join();
    producer2.join();
    producer3.join();

    consumer1.join();
    consumer2.join();
    consumer3.join();
}

//--------------------------------------------------------------------------

}  // namespace producer_consumer

int main() {
    using namespace producer_consumer;

    Test1();
    Test2();
    Test3();
    Test4();

    return 0;
}
