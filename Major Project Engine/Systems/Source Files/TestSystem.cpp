#include "TestSystem.h"

TestSystem::TestSystem()
{}

TestSystem::~TestSystem() 
{
	std::cout << "Test System destructor called" << std::endl;
}

bool TestSystem::Load(const Content * content)
{
	//testing_field = std::vector<int>();
	////srand(time(NULL));

	//for (int i = 0; i < 100; i++)
	//{
	//	testing_field.emplace_back(std::rand() % 100 + 1);
	//}
	//results[0] = int(0);
	//results[1] = int(0);
	//results[2] = int(0);
	//results[3] = int(0);
	//Job * parent_job = new Job(bind_function(&TestSystem::return_process, this));
	//
	//for (int i = 0; i < ThreadManager::Instance().get_num_threads(); i++)
	//{
	//	std::vector<int> testing(testing_field.begin() + (i * (100 / ThreadManager::Instance().get_num_threads())), testing_field.begin() + ((i + 1) * (100 / ThreadManager::Instance().get_num_threads())));
	//	ThreadManager::Instance().register_job(new Job(bind_function(&TestSystem::process, this), new InitialContent(testing, &results[i])), parent_job);
	//}
	//ThreadManager::Instance().register_job(parent_job);
	return true;
}

void TestSystem::Close(const Content * content)
{

}

bool TestSystem::process(const Content * content)
{
	const InitialContent * IContent = dynamic_cast<const InitialContent*>(content);
	int total = 0, count = 0;
	for (int i : IContent->copy_section)
	{
		total += i;
		count++;
	}
	*IContent->result = total;

	IContent = nullptr;
	return true;
}

bool TestSystem::return_process(const Content * content)
{
	if (content == nullptr) {}

	return true;
}
