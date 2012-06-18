/**
 */
#ifndef	OBJECT_H
#define	OBJECT_H

#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>

/**
 * @class Object
 * @brief The base object from which all objects inherit.
 */
extern const void * Object;		/* new(Object); */

/**
 * @class Object
 * @fn Object *new(void *Object);
 * @brief Create an new Object.
 * @param Object The object class.
 * @return An Object pointer.
 */
void * new (const void * class, ...);

/**
 * @brief Delete any object.
 * @param self A pointer to the instance to be deleted.
 */
void delete (void * self);

/**
 * @brief Return the class sof any object.
 * @param self The object.
 * @return A pointer to the class
 */
const void * classOf (const void * self);

/**
 * @brief
size_t sizeOf (const void * self);

void * ctor (void * self, va_list * app);
void * dtor (void * self);
int differ (const void * self, const void * b);
int puto (const void * self, FILE * fp);

/* new(Class, "name", super, size, sel, meth, ... 0); */
/**
 * @class Class
 * @brief The base object from which all classes inherit
 */
extern const void * Class;

/**
 * @fn Class * new(void* Class, char *name, void *super, size_t size, meth, voidf *fnc, ... 0)
 * @brief Create a new Class.
 * @param Class Pointer to Class.
 * @param name The name of the class.
 * @param super The class which this class inherits from.  Should be Class for those that do not inherit from user defined classes.
 * @param size The size of class structure.
 * @param meth A pointer to a method to be overidden.
 * @param fnc The new function.
 * @return A pointer to a new class.
 */

const void * super (const void * self);	/* class' superclass */

#endif
