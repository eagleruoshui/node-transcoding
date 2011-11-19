#include "task.h"

extern "C" {
#include "libavformat/avformat.h"
}

using namespace transcode;
using namespace v8;

static Persistent<String> _task_timestamp_symbol;
static Persistent<String> _task_duration_symbol;
static Persistent<String> _task_timeElapsed_symbol;
static Persistent<String> _task_timeEstimated_symbol;
static Persistent<String> _task_timeRemaining_symbol;

static Persistent<FunctionTemplate> _task_ctor;

void Task::Init(Handle<Object> target) {
  HandleScope scope;

  _task_timestamp_symbol = NODE_PSYMBOL("timestamp");
  _task_duration_symbol = NODE_PSYMBOL("duration");
  _task_timeElapsed_symbol = NODE_PSYMBOL("timeElapsed");
  _task_timeEstimated_symbol = NODE_PSYMBOL("timeEstimated");
  _task_timeRemaining_symbol = NODE_PSYMBOL("timeRemaining");

  Local<FunctionTemplate> ctor = FunctionTemplate::New(New);
  ctor->InstanceTemplate()->SetInternalFieldCount(1);
  ctor->SetClassName(String::NewSymbol("Task"));

  NODE_SET_PROTOTYPE_METHOD(ctor, "start", Start);
  NODE_SET_PROTOTYPE_METHOD(ctor, "stop", Stop);

  NODE_SET_PROTOTYPE_ACCESSOR(ctor, "source", GetSource);
  NODE_SET_PROTOTYPE_ACCESSOR(ctor, "target", GetTarget);
  NODE_SET_PROTOTYPE_ACCESSOR(ctor, "options", GetOptions);
  NODE_SET_PROTOTYPE_ACCESSOR(ctor, "progress", GetProgress);

  _task_ctor = Persistent<FunctionTemplate>::New(ctor);
  target->Set(String::NewSymbol("Task"), _task_ctor->GetFunction());
}

Handle<Value> Task::New(const Arguments& args)
{
  HandleScope scope;
  Local<Object> source = args[0]->ToObject();
  Local<Object> target = args[1]->ToObject();
  Local<Object> options = args[2]->ToObject();
  Task* task = new Task(source, target, options);
  task->Wrap(args.This());
  return scope.Close(args.This());
}

Task::Task(
    Handle<Object> source, Handle<Object> target, Handle<Object> options) {
  HandleScope scope;

  this->source = Persistent<Object>::New(source);
  this->target = Persistent<Object>::New(target);
  this->options = Persistent<Object>::New(options);
}

Task::~Task() {
}

Handle<Value> Task::GetSource(Local<String> property,
    const AccessorInfo& info) {
  Task* task = ObjectWrap::Unwrap<Task>(info.This());
  HandleScope scope;
  return scope.Close(task->source);
}

Handle<Value> Task::GetTarget(Local<String> property,
    const AccessorInfo& info) {
  Task* task = ObjectWrap::Unwrap<Task>(info.This());
  HandleScope scope;
  return scope.Close(task->target);
}

Handle<Value> Task::GetOptions(Local<String> property,
    const AccessorInfo& info) {
  Task* task = ObjectWrap::Unwrap<Task>(info.This());
  HandleScope scope;
  return scope.Close(task->options);
}

Handle<Value> Task::GetProgress(Local<String> property,
    const AccessorInfo& info) {
  Task* task = ObjectWrap::Unwrap<Task>(info.This());
  HandleScope scope;

  Local<Object> result = Object::New();

  result->Set(_task_timestamp_symbol, Number::New(1));
  result->Set(_task_duration_symbol, Number::New(2));
  result->Set(_task_timeElapsed_symbol, Number::New(3));
  result->Set(_task_timeEstimated_symbol, Number::New(4));
  result->Set(_task_timeRemaining_symbol, Number::New(5));

  return scope.Close(result);
}

Handle<Value> Task::Start(const Arguments& args) {
  Task* task = ObjectWrap::Unwrap<Task>(args.This());
  HandleScope scope;

  printf("start\n");

  // TODO: emit events
  Handle<Value> argv[] = {
    String::New("end"),
  };
  node::MakeCallback(args.This(), "emit", countof(argv), argv);

  return scope.Close(Undefined());
}

Handle<Value> Task::Stop(const Arguments& args) {
  Task* task = ObjectWrap::Unwrap<Task>(args.This());
  HandleScope scope;

  printf("stop\n");

  return scope.Close(Undefined());
}
