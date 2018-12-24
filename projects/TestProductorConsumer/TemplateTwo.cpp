#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <cassert>

// �ο���http://www.cnblogs.com/haippy/p/3284540.html

namespace producer_consumer {

const int kRepositorySize = 10;
const int kDataCount = 100;

struct Repository
{
    int datas[kRepositorySize];
    int read_position;
    int write_position;

    // ����ֿ��ǹ�����Դ��Ҳ����˵�����������߶�дͬһ���ֿ⣬����������������Ҫ��ͬһ����������ʵ���̰߳�ȫ��
    // �����������������Repository�������������ʡ�
    std::mutex mutex;

    // ������������ʵ���߳�ͬ����ÿ����ɫ��ӵ�������Լ�ִ�������ı�����
    // ������������˵����Ϊ����ִ��������������������˵��Ϊ����ִ��������
    std::condition_variable not_full_cv;
    std::condition_variable not_empty_cv;

    // ���ڶ������������
    int consumed_count;  // �����ѵ�������
    std::mutex consumer_mutex;  // ��������֮��Ļ�����������consumed_count���̰߳�ȫ��

    // ���ڶ������������
    int produced_count;  // ��������������
    std::mutex producer_mutex;  // ��������֮��Ļ�����������produced_count���̰߳�ȫ��

    // ׼�����������ݡ�
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

    // ѭ��������һ��Ԫ���ǲ���ֵ�ģ������ж϶����������������ֿ�������ʼ�ȴ���
    while ((repository.write_position + 1) % kRepositorySize == repository.read_position)
    {
        repository.not_full_cv.wait(lock);  // wait not full, �ȴ��ֿⲻΪ����
    }

    repository.datas[repository.write_position++] = data;
    repository.write_position %= kRepositorySize;

    // ���������������ݣ���ô�ֿⲻΪ�գ������֪ͨ������ȥ���ѡ�
    repository.not_empty_cv.notify_all();
}

static void Consumer(Repository& repository)
{
    std::unique_lock<std::mutex> lock(repository.mutex);

    // ѭ�����У�����Ϊ�յ��������ֿ�Ϊ����ʼ�ȴ���
    while (repository.read_position == repository.write_position)
    {
        repository.not_empty_cv.wait(lock);  // wait not empty, �ȴ��ֿⲻΪ�ա�
    }

    int data = repository.datas[repository.read_position++];
    repository.read_position %= kRepositorySize;

    // ���������ݣ���ô�ֿⲻΪ���������֪ͨ������ȥ������
    repository.not_full_cv.notify_all();

    std::cout << "---Thread-" << std::this_thread::get_id() << ": consume: " << data << std::endl;
}

//-------------------------------------------------------------------------------
// 1.��������-�������ߡ�
// �������� - ��������ģ����ֻ��һ�������ߺ�һ�������ߣ������߲�ͣ������Ʒ���з����Ʒ��
// ��������Ӳ�Ʒ����ȡ�߲�Ʒ����Ʒ���ݻ������ƣ�ֻ������һ����Ŀ�Ĳ�Ʒ�����������������Ʒ���ٶȹ��죬
// ����Ҫ�ȴ�������ȡ�߲�Ʒ֮�󣬲�Ʒ�ⲻΪ�ղ��ܼ�������Ʒ���з����µĲ�Ʒ���෴�����������ȡ�߲�Ʒ���ٶȹ��죬
// ��������ٲ�Ʒ����û�в�Ʒ��ʹ�õ��������ʱ��Ҫ�ȴ������߷���һ����Ʒ�������߲��ܼ���������

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
// 2.��������-�������ߡ�
// ��������-��������ģ���п���������������ͬʱ�Ӳ�Ʒ����ȡ�߲�Ʒ��
// ���Գ��˱�����Ʒ���ڶ����д�߳��»���֮�⣬����Ҫά��������ȡ�߲�Ʒ�ļ�������

static void StartConsume2(Repository& repository)
{
    while (true)
    {
        // ���������֮�以�⣬��֤consumed_count�̰߳�ȫ����������ȷ֪�����ѵĲ�Ʒ��������
        std::unique_lock<std::mutex> lock(repository.consumer_mutex);
        // �Ѿ����ѵĲ�Ʒ�������ڵ����ܲ�Ʒ����ʱ���˳���
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
// 3.��������-�������ߡ�
// ��������-��������ģ���п���������������ͬʱ���Ʒ���з����Ʒ��
// ���Գ��˱�����Ʒ���ڶ����д�߳��»���֮�⣬����Ҫά�������߷����Ʒ�ļ�������

static void StartProduce3(Repository& repository)
{
    while (true)
    {
        // ���������֮�以�⣬��֤produced_count�̰߳�ȫ����������ȷ֪�������˶��ٲ�Ʒ��
        std::unique_lock<std::mutex> lock(repository.producer_mutex);
        // �����Ĳ�Ʒ�������ڼƻ����ܲ�Ʒ����ʱ���˳���
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
// 4.��������-�������ߡ�
// ��ģ�Ϳ���˵��ǰ������ģ�͵��ۺϣ�������Ҫά���������������ֱ�����������������Ʒ����Ŀ
// ����������ȡ�߲�Ʒ����Ŀ������Ҳ��Ҫ������Ʒ���ڶ�������ߺͶ�������߻���ط��ʡ�

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
