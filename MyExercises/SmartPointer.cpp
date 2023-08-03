#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG


#include <iostream>
#include <vector>
#include <algorithm>

void exercise01() {
	class X
	{
	public:
		X() : m_value{ 123 } {}
		int getValue() const { return m_value; }
		
		private:
		int m_value;
	};

	std::shared_ptr<X> pA{ new X{} };
	std::shared_ptr<X> pB;
	std::shared_ptr<X> pC;

	pB = pA;
	pC = std::shared_ptr<X>(pB);
	pC = nullptr;
	int value = (*pB).getValue();
	std::cout << "Value: " << value << std::endl;
	
}

void exercise03() {
	struct X;
	struct Y;
	
	struct X
	{
	    std::weak_ptr<Y> m_sp_Y{};
	};
	
	struct Y
	{
	    std::weak_ptr<X> m_sp_X{};
	};
	
	//void test()
	{
	    {
	        std::shared_ptr<X> sp_X{ std::make_shared<X>() };
	        std::shared_ptr<Y> sp_Y{ std::make_shared<Y>() };
	
			sp_X->m_sp_Y = sp_Y;
	        sp_Y->m_sp_X = sp_X;

			std::cout << "UseCount sp1: " << sp_X.use_count() << std::endl;
			std::cout << "   UseCount sp1->m_spY: " << sp_X->m_sp_Y.use_count() << std::endl;

			std::cout << "UseCount sp2: " << sp_Y.use_count() << std::endl;
			std::cout << "   UseCount sp2->m_spX: " << sp_Y->m_sp_X.use_count() << std::endl;
	    }
	}
}

void exercise04() {
	class LightAndSafeWatcher {
	private:
		std::weak_ptr<int> m_ptr;

	public:
		LightAndSafeWatcher() {};

		void watch(const std::shared_ptr<int>& sp)
		{
			m_ptr = sp;
		}

		int currentValue() const
		{

			auto tmpptr = m_ptr.lock();
			if (tmpptr != nullptr) {
				return *tmpptr;
			}
			return -1;

		}
	};
	
	//void test()
	{
		LightAndSafeWatcher watcher;
	
		{
	        std::shared_ptr<int> sp = std::make_shared<int>(123);
	        watcher.watch(sp);
	        std::cout << "Value: " << watcher.currentValue() << std::endl;
	    }
	
	std::cout << "Value: " << watcher.currentValue() << std::endl;
	}
}

void fibonacci() {
	std::pair<int, int> tmpVal {0, 1};
	std::vector<int> fibo(20);

	std::generate(
		fibo.begin(),
		fibo.end(),
		[&tmpVal] {
			int tmp0 = tmpVal.first;
			int tmp1 = tmpVal.second;
			tmpVal.first = tmpVal.second;
			tmpVal.second = tmp0 + tmp1;
			return tmpVal.second;
		}
	);

	for (auto v : fibo) {
		std::cout << v << ", " << std::endl;
	}

}

int main() {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);


	//exercise01();
	//exercise02();
	//exercise04();
	fibonacci();
}