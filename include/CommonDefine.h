#pragma once

#if defined(_MSC_VER)
#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE (1)
#endif
#pragma warning(disable : 4996)
#endif

#define DECLARE_PRIVATE(Class) \
inline Class##Private* d_func() { return reinterpret_cast<Class##Private *>(d_ptr); } \
inline const Class##Private* d_func() const { return reinterpret_cast<const Class##Private *>(d_ptr); } \
friend class Class##Private;

#define DECLARE_PUBLIC(Class) \
inline Class* q_func() { return static_cast<Class *>(q_ptr); } \
inline const Class* q_func() const { return static_cast<const Class *>(q_ptr); } \
friend class Class;

#define DEFINE_PRIVATE(Class) \
	Class##Private* const d_ptr;

#define DEFINE_PUBLIC(Class) \
	Class* const q_ptr;

#define Q_D(Class) Class##Private * const d = d_func()
#define Q_Q(Class) Class * const q = q_func()



#define DEFINE_DATA(Class) \
	Class##Data* const d;

#define DECLARE_DATA(Class) \
	inline Class##Data* d_func() { return reinterpret_cast<Class##Data *>(d); } \
	inline const Class##Data* d_func() const { return reinterpret_cast<const Class##Data *>(d); }

#define M_D(Class) Class##Data* const d = d_func()

#define LIB_NAME "Framework"