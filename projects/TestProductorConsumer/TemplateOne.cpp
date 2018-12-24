#include <thread>
#include <iostream>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <atomic>

using namespace std;

int main()
{
	queue<long> buffer;
	mutex lockBuffer;                        //���������ź���
	volatile bool bStart = true; //ʹ���������ѹ��̵Ľ���

	condition_variable_any cndConsumer;//condition variable
	condition_variable_any cndProductor;

	thread ThreadProducteur([&]()//�������߳�
	{
		std::atomic<long> atomicLong;//��interlock�Ĳ�������ԭ�ӵ�
		atomicLong = 1;

		while (bStart)
		{

			std::this_thread::sleep_for(chrono::milliseconds(15));

			long element = atomicLong.fetch_add(1);//��1��
			cout << "Production element :" << element << "buffer size :" << buffer.size() << endl;

			lockBuffer.lock();
			while (buffer.size() == 10 && bStart)
			{
				cndProductor.wait(lockBuffer);//��2��
			}

			if (bStart == false)
			{
				lockBuffer.unlock();
				cndConsumer.notify_one();//��3��
				break;
			}

			buffer.push(element);
			lockBuffer.unlock();
			cndConsumer.notify_one();
		}

	});

	thread ThreadConsommateur([&]()
	{

		while (bStart)
		{

			lockBuffer.lock();
			while (buffer.empty() && bStart)
			{
				cndConsumer.wait(lockBuffer);
			}

			if (bStart == false && buffer.empty())
			{
				lockBuffer.unlock();
				cndProductor.notify_one();
				break;
			}

			long element = buffer.front();
			buffer.pop();

			lockBuffer.unlock();
			cndProductor.notify_one();

			cout << "Consommation element :" << element << " size :" << buffer.size() << endl;
		}

	});

	std::cout << "Pour arreter pressez [ENTREZ]" << std::endl;

	getchar();

	std::cout << "Arret demande" << endl;
	bStart = false;

	ThreadProducteur.join();
	ThreadConsommateur.join();

	cout << "Main Thread" << endl;
	return 0;

}
