#include "stdafx.h"
#include "CppUnitTest.h"

#include "MetaType.h"
#include "Component.h"
#include "GameObject.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

static const char alphanum[] =
"0123456789"
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz";

constexpr unsigned int stressTestLength = 1000;

class TestClass1
{
public:
  REFLECT_CLASS;
  int someData;
  double someOtherData;
};

INIT_REFLECTION(TestClass1);

class TestClass2
{
public:
  REFLECT_CLASS;
  long definatelyDifferentData;
};

INIT_REFLECTION(TestClass2);

namespace UnitTests
{		
	TEST_CLASS(MetaClassTests)
	{
	public:
		TEST_METHOD(MetaClass_CheckNames)
		{
      Assert::AreEqual(TestClass1::MetaInfo.Name.c_str(), "TestClass1");
      Assert::AreEqual(TestClass2::MetaInfo.Name.c_str(), "TestClass2");
		}
    TEST_METHOD(MetaClass_CheckIDDifferent)
    {
      Assert::AreNotEqual(TestClass1::MetaInfo.ID, TestClass2::MetaInfo.ID);
    }
    TEST_METHOD(MetaClass_CheckTwoInstancesEqual)
    {
      TestClass1 obj1;
      TestClass1 obj2;

      Assert::AreEqual(obj1.MetaInfo.ID, obj2.MetaInfo.ID);
    }
    TEST_METHOD(MetaClass_CheckTwoInstancesDifferent)
    {
      TestClass1 obj1;
      TestClass2 obj2;

      Assert::AreNotEqual(obj1.MetaInfo.ID, obj2.MetaInfo.ID);
    }
	};

  class TestComponentA : public Beans::Component
  {
  public:
    TestComponentA(Beans::GameObject* owner) : Component(owner) {};

    int someData;
  };

  TEST_CLASS(GameObjectTests)
  {
  public:
    TEST_METHOD(GameObject_BasicNameTest)
    {
      Beans::GameObject go("Test Object");

      Assert::AreEqual(go.Name.c_str(), "Test Object");
    }
    TEST_METHOD(GameObject_NameStressTest)
    {
      for (int i = 0; i < stressTestLength; ++i)
      {
        std::string testName;

        constexpr unsigned int testNameLength = 64;
        for (int j = 0; j < testNameLength; ++j)
        {
          testName.push_back(alphanum[rand() % sizeof(alphanum) - 1]);
        }

        Beans::GameObject go(testName);

        Assert::AreEqual(go.Name, testName);
      }
    }
    TEST_METHOD(GameObject_VerfifyUniqueIDs)
    {
      std::vector<Beans::GameObject> objects;

      for (int i = 0; i < stressTestLength; ++i)
      {
        constexpr unsigned int testNameLength = 32;
        std::string testName;
        for (int j = 0; j < testNameLength; ++j)
        {
          testName.push_back(alphanum[rand() % sizeof(alphanum) - 1]);
        }
        Beans::GameObject object(testName);

        for (const Beans::GameObject& testAgainst : objects)
        {
          Assert::AreNotEqual(object.ID, testAgainst.ID);
        }

        objects.push_back(object);
      }
    }
    TEST_METHOD(GameObject_InstanceCounter)
    {
      unsigned long startCount = Beans::GameObject::GetActiveObjects();
      std::vector<Beans::GameObject> objects;

      for (int i = 0; i < stressTestLength; ++i)
      {
        objects.emplace_back(std::to_string(i));
      }

      Assert::AreEqual(startCount + (unsigned long)objects.size(), Beans::GameObject::GetActiveObjects());

      while (!objects.empty())
      {
        objects.pop_back();
      }

      Assert::AreEqual(startCount, Beans::GameObject::GetActiveObjects());
    }
  };
}