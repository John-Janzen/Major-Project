#include "TestSystem.h"

TestSystem::TestSystem(TaskManager & tm, SceneManager & sm)
	: System(tm, sm)
{}

TestSystem::~TestSystem() 
{
	std::cout << "Test System destructor called" << std::endl;
}

bool TestSystem::Load()
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
	//Job * parent_job = new Job(bind_function(&TestSystem::ReturnProcess, this));
	//
	//for (int i = 0; i < ThreadManager::Instance().GetNumThreads(); i++)
	//{
	//	std::vector<int> testing(testing_field.begin() + (i * (100 / ThreadManager::Instance().GetNumThreads())), testing_field.begin() + ((i + 1) * (100 / ThreadManager::Instance().GetNumThreads())));
	//	ThreadManager::Instance().RegisterJob(new Job(bind_function(&TestSystem::Process, this), new InitialContent(testing, &results[i])), parent_job);
	//}
	//ThreadManager::Instance().RegisterJob(parent_job);
	return JOB_COMPLETED;
}

void TestSystem::Close(void* content)
{

}

void TestSystem::HandleEvent(const EventType & e, void * data)
{
}

bool TestSystem::Process(void* content)
{
	//const InitialContent * IContent = dynamic_cast<const InitialContent*>(content);
	/*InitialContent * IContent = static_cast<InitialContent*>(content);
	int total = 0, count = 0;
	for (int i : IContent->copy_section)
	{
		total += i;
		count++;
	}
	*IContent->result = total;

	IContent = nullptr;*/
	return true;
}

bool TestSystem::ReturnProcess(void* content)
{
	if (content == nullptr) {}

	return true;
}
