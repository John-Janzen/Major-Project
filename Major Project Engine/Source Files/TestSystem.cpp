#include "TestSystem.h"

TestSystem::TestSystem(const std::shared_ptr<ThreadManager> & thread_manager)
	: t_manager(thread_manager)
{}

TestSystem::~TestSystem() {}

bool TestSystem::Load()
{
	testing_field = std::vector<int>();
	srand(time(NULL));

	for (int i = 0; i < 100; i++)
	{
		testing_field.emplace_back(std::rand() % 100 + 1);
	}
	std::unique_ptr<Job> jobs[4] = {nullptr};

	
	for (int i = 0; i < t_manager->get_num_threads(); i++)
	{	
		std::vector<int> testing(testing_field.begin() + (i * (100 / t_manager->get_num_threads())), testing_field.begin() + ((i + 1) * (100 / t_manager->get_num_threads())));
		jobs[i] = std::make_unique<Job>(NULL_TYPE, std::bind(&TestSystem::process, std::ref(*this), std::placeholders::_1), std::make_shared<InitialContent>(testing));
	}
	//t_manager->register_job();
	return true;
}

void TestSystem::Close()
{

}

void TestSystem::process(const std::shared_ptr<Content> & content)
{
	std::shared_ptr<InitialContent> IContent = std::static_pointer_cast<InitialContent>(content);



	IContent.reset();
}
