#ifndef MRB_MACROS_H

#define DEVMODE 1

#ifndef DEVMODE

#define SEND(name, value, method, runtime)\
  mrb_value name = mrb_funcall(runtime, value, #method, 0);
#define SEND_1(name, value, method, arg_one, runtime)\
  mrb_value name = mrb_funcall(runtime, value, #method, 1, arg_one);
#define SEND_2(name, value, method, arg_one, arg_two, runtime)\
  mrb_value name = mrb_funcall(runtime, value, #method, 2, arg_one, arg_two);

#else

#define CHECK_FOR_EXCEPTION(runtime)\
  do {\
    if (runtime->exc) {\
      mrb_value exception = mrb_funcall(runtime,\
					mrb_obj_value(runtime->exc),\
					"inspect",\
					0);\
      SDL_Log("%s", RSTRING_PTR(exception));\
      exit(-1);\
    }\
  } while (0);

#define SEND(name, value, method, runtime)\
  mrb_value name = mrb_funcall(runtime, value, #method, 0);\
  CHECK_FOR_EXCEPTION(runtime);

#define SEND_1(name, value, method, arg_one, runtime)\
  mrb_value name = mrb_funcall(runtime, value, #method, 1, arg_one);\
  CHECK_FOR_EXCEPTION((runtime);

#define SEND_2(name, value, method, arg_one, arg_two, runtime)\
  mrb_value name = mrb_funcall(runtime, value, #method, 2, arg_one, arg_two); \
  CHECK_FOR_EXCEPTION(runtime);

#endif //DEVMODE

#endif //MRB_MACROS_H
