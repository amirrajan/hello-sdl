#ifndef MALLOC_MACROS_H
#define MALLOC_MACROS_H
#define MALLOC(type, variable_name) type * variable_name = (type *)malloc(sizeof(type))
#define MALLOCA(type) (type *)malloc(sizeof(type))
#define MALLOCS(type, variable_name, count) type * variable_name = (type **)malloc(sizeof(type *) * count)
#define MALLOCSA(type, count) (type **)malloc(sizeof(type *) * count)
#endif
