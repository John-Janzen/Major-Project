#include "TestSystem.h"

TestSystem::TestSystem()
{}

TestSystem::~TestSystem() {}

bool TestSystem::Load()
{
	testing_field = std::vector<int>();
	//srand(time(NULL));

	for (int i = 0; i < 100; i++)
	{
		testing_field.emplace_back(std::rand() % 100 + 1);
	}
	std::unique_ptr<Job> jobs[4] = { nullptr };

	std::unique_ptr<Job> parent_job = std::make_unique<Job>(NULL_TYPE, std::bind(&TestSystem::return_process, std::ref(*this), std::placeholders::_1));
	for (int i = 0; i < ThreadManager::Instance().get_num_threads(); i++)
	{	
		std::vector<int> testing(testing_field.begin() + (i * (100 / ThreadManager::Instance().get_num_threads())), testing_field.begin() + ((i + 1) * (100 / ThreadManager::Instance().get_num_threads())));
		jobs[i] = std::make_unique<Job>(NULL_TYPE, std::bind(&TestSystem::process, std::ref(*this), std::placeholders::_1), std::make_shared<InitialContent>(testing));
		jobs[i]->add_observer(&parent_job);
		ThreadManager::Instance().register_job(std::move(jobs[i]));
	}
	ThreadManager::Instance().register_job(std::move(parent_job));
	return true;
}

void TestSystem::Close()
{

}

void TestSystem::process(const std::shared_ptr<Content> & content)
{
	std::shared_ptr<InitialContent> IContent = std::static_pointer_cast<InitialContent>(content);
	int total = 0, count = 0;
	for (int i : IContent->copy_section)
	{
		total += i;
		count++;
	}

	IContent.reset();
}

void TestSystem::return_process(const std::shared_ptr<Content> & content)
{
	if (content == nullptr) {}


}
