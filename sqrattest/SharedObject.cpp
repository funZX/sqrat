#include <gtest/gtest.h>
#include <sqrat.h>
#include "Fixture.h"

struct CxxObject {
  int a;

  CxxObject() : a(0) { }
  void set(int v) { a = v; }
  int get() const { return a; }
};

char const script_code_in[] = "\
function RunTestA(a)\
{\
a.set(123);\
return a;\
}\
\
function RunTestB(a)\
{\
a.set(-1337);\
return a;\
}\
";

char const script_code_out[] = "\
function RunTestA()\
{\
a <- CxxObject();\
a.set(123);\
return a;\
}\
\
function RunTestB(a)\
{\
a.set(-1337);\
return a;\
}\
";

using namespace Sqrat;

TEST_F(SqratTest, SharedObjectIn) {
  std::shared_ptr< Sqrat::ObjectReference<CxxObject> > obj2;

  DefaultVM::Set(vm);

  Class<CxxObject> cls(vm, _SC("CxxObject"));
  cls.Func(_SC("set"), &CxxObject::set);
  cls.Func(_SC("get"), &CxxObject::get);
  RootTable(vm).Bind(_SC("CxxObject"), cls);

  Script script;
  std::string err;
  if (!script.CompileString(script_code_in, err)) {
    FAIL() << _SC("Compile failed: ") << err;
  }
  if (!script.Run(err)) {
    FAIL() << _SC("Script failed: ") << err;
  }

  auto obj = std::make_shared<CxxObject>();

  Function testFunctionA = RootTable().GetFunction(_SC("RunTestA"));
  ASSERT_FALSE(testFunctionA.IsNull());
  obj2 = testFunctionA.Evaluate< Sqrat::ObjectReference<CxxObject> >(obj);
  ASSERT_FALSE(nullptr == obj2.get()) << _SC("Function call failed.");
  ASSERT_FALSE(nullptr == obj2->GetPointer()) << _SC("Returned object is null.");
  EXPECT_EQ(obj2->GetPointer(), obj.get()) << _SC("Objects are not the same.");
  EXPECT_EQ(obj->get(), 123) << _SC("Objects do not retain the same value.");
  EXPECT_EQ(obj2->GetPointer()->get(), 123) << _SC("Objects do not retain the same value.");

  Function testFunctionB = RootTable().GetFunction(_SC("RunTestB"));
  ASSERT_FALSE(testFunctionB.IsNull());
  obj2 = testFunctionB.Evaluate< Sqrat::ObjectReference<CxxObject> >(obj2->GetSharedObject());
  ASSERT_FALSE(nullptr == obj2.get()) << _SC("Function call failed.");
  ASSERT_FALSE(nullptr == obj2->GetPointer()) << _SC("Returned object is null.");
  EXPECT_EQ(obj2->GetPointer(), obj.get()) << _SC("Objects are not the same.");
  EXPECT_EQ(obj->get(), -1337) << _SC("Objects do not retain the same value.");
  EXPECT_EQ(obj2->GetPointer()->get(), -1337) << _SC("Objects do not retain the same value.");
}

TEST_F(SqratTest, SharedObjectOut) {
  std::shared_ptr< Sqrat::ObjectReference<CxxObject> > obj;
  std::shared_ptr< Sqrat::ObjectReference<CxxObject> > obj2;

  DefaultVM::Set(vm);

  Class<CxxObject> cls(vm, _SC("CxxObject"));
  cls.Func(_SC("set"), &CxxObject::set);
  cls.Func(_SC("get"), &CxxObject::get);
  RootTable(vm).Bind(_SC("CxxObject"), cls);

  Script script;
  std::string err;
  if (!script.CompileString(script_code_out, err)) {
    FAIL() << _SC("Compile failed: ") << err;
  }
  if (!script.Run(err)) {
    FAIL() << _SC("Script failed: ") << err;
  }

  Function testFunctionA = RootTable().GetFunction(_SC("RunTestA"));
  ASSERT_FALSE(testFunctionA.IsNull());
  obj = testFunctionA.Evaluate< Sqrat::ObjectReference<CxxObject> >();
  ASSERT_FALSE(nullptr == obj.get()) << _SC("Function call failed.");
  ASSERT_FALSE(nullptr == obj->GetPointer()) << _SC("Returned object is null.");
  EXPECT_EQ(obj->GetPointer()->get(), 123) << _SC("Objects do not retain the same value.");

  Function testFunctionB = RootTable().GetFunction(_SC("RunTestB"));
  ASSERT_FALSE(testFunctionB.IsNull());
  obj2 = testFunctionB.Evaluate< Sqrat::ObjectReference<CxxObject> >(obj->GetSharedObject());
  ASSERT_FALSE(nullptr == obj2.get()) << _SC("Function call failed.");
  ASSERT_FALSE(nullptr == obj2->GetPointer()) << _SC("Returned object is null.");
  EXPECT_EQ(obj2->GetPointer(), obj->GetPointer()) << _SC("Objects are not the same.");
  EXPECT_EQ(obj->GetPointer()->get(), -1337) << _SC("Objects do not retain the same value.");
  EXPECT_EQ(obj2->GetPointer()->get(), -1337) << _SC("Objects do not retain the same value.");
}
