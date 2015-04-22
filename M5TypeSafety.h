//
//  M5TypeSafety.h
//  M5TypeSafety
//

#import <Foundation/Foundation.h>

#pragma mark - M5TypeSafety -

#pragma mark Macros

#define M5MacroStringify(K...) (@#K)

#ifdef DEBUG
/* Asserts with message as content of assertion. */
#define M5Assert(CASE) NSAssert(CASE, @"%@", M5MacroStringify(CASE));

/* Asserts that object is not nil, returns object. */
#define M5AssertValue(OBJECT) \
({ \
    id M5AssertValue = OBJECT; \
    NSAssert(M5AssertValue != nil, @"'%@' can't be nil.", M5MacroStringify(OBJECT)); \
    M5AssertValue; \
})

/* Asserts that pointer is not NULL, returns pointer. */
#define M5AssertCValue(OBJECT) \
({ \
    void *M5AssertCValue = OBJECT; \
    NSAssert(M5AssertCValue != NULL, @"'%@' can't be NULL.", M5MacroStringify(OBJECT)); \
    M5AssertCValue; \
})

/* Asserts that object is subclass of one of the provided classes, returns object. */
#define M5AssertClass(OBJECT, ...) \
({ \
    id M5AssertClass = OBJECT; \
    M5AssertValue(M5AssertClass); \
    NSAssert(M5ObjectIsOfAnyClass(M5AssertClass, @[__VA_ARGS__]), @"'%@' must be one of: %@", M5MacroStringify(OBJECT), M5ClassesDescription(@[__VA_ARGS__])); \
    M5AssertClass; \
})

/* Asserts that property on object equals one of the provided values, returns object. */
#define M5AssertProperty(OBJECT, PROPERTY, ...) \
({ \
    id M5AssertProperty = OBJECT; \
    M5AssertValue(M5AssertProperty); \
    NSAssert(M5ObjectPropertyEqualsAnyValue(M5AssertProperty, PROPERTY, @[__VA_ARGS__]), @"'%@' on '%@' must equal one of: %@", NSStringFromSelector(PROPERTY), M5MacroStringify(OBJECT), M5ObjectsDescription(@[__VA_ARGS__])); \
    M5AssertProperty; \
})

/* Asserts that object conforms to all of the provided protocols, returns object. */
#define M5AssertProtocol(OBJECT, ...) \
({ \
    id M5AssertProtocol = OBJECT; \
    M5AssertValue(M5AssertProtocol); \
    NSAssert(M5ObjectConformsToProtocols(M5AssertProtocol, @[__VA_ARGS__]), @"'%@' must conform to all of: %@", M5MacroStringify(OBJECT), M5ProtocolsDescription(@[__VA_ARGS__])); \
    M5AssertProtocol; \
})

/* Asserts that object (string, array, dictionary, etc.) is not null/empty. */
#define M5AssertNotEmpty(OBJECT) \
({ \
    id M5AssertNotEmpty = OBJECT; \
    NSAssert(M5NotEmpty(M5AssertNotEmpty), @"'%@' can't be empty.", M5MacroStringify(OBJECT)); \
    M5AssertNotEmpty; \
})

/* Asserts that the objects are each subclasses of one of the provided classes, returns object. */
#define M5AssertContentClass(OBJECTS, ...) \
({ \
    id<NSFastEnumeration> M5AssertContentClass = OBJECTS; \
    M5AssertNotEmpty(M5AssertContentClass); \
    NSAssert(M5ObjectsAreOfAnyClass(M5AssertContentClass, @[__VA_ARGS__]), @"Contents of '%@' must be one of: %@", M5MacroStringify(OBJECTS), M5ClassesDescription(@[__VA_ARGS__])); \
    M5AssertContentClass; \
})

/* Asserts that the property on each of the objects equals one of the provided values. */
#define M5AssertContentProperty(OBJECTS, PROPERTY, ...) \
({ \
    id<NSFastEnumeration> M5AssertContentProperty = OBJECTS; \
    M5AssertNotEmpty(M5AssertContentProperty); \
    NSAssert(M5ObjectsPropertyEqualsAnyValue(M5AssertContentProperty, PROPERTY, @[__VA_ARGS__]), @"'%@' on contents of '%@' must equal one of: %@", NSStringFromSelector(PROPERTY), M5MacroStringify(OBJECT), M5ObjectsDescription(@[__VA_ARGS__])); \
    M5AssertContentProperty; \
})

/* Asserts that each of the objects conforms to all of the provided protocols. */
#define M5AssertContentProtocol(OBJECTS, ...) \
({ \
    id<NSFastEnumeration> M5AssertContentProtocol = OBJECTS; \
    M5AssertNotEmpty(M5AssertContentProtocol); \
    NSAssert(M5ObjectsConformToProtocols(M5AssertContentProtocol, @[__VA_ARGS__]), @"Contents of '%@' must conform to all of: %@", M5MacroStringify(OBJECTS), M5ProtocolsDescription(@[__VA_ARGS__])); \
    M5AssertContentProtocol; \
})
#else
#define M5Assert(CASE)
#define M5AssertValue(OBJECT) OBJECT
#define M5AssertCValue(OBJECT) OBJECT
#define M5AssertClass(OBJECT, ...) OBJECT
#define M5AssertProperty(OBJECT, PROPERTY, ...) OBJECT
#define M5AssertProtocol(OBJECT, ...) OBJECT
#define M5AssertNotEmpty(OBJECT) OBJECT
#define M5AssertContentClass(OBJECTS, ...) OBJECTS
#define M5AssertContentProperty(OBJECTS, PROPERTY, ...) OBJECTS
#define M5AssertContentProtocol(OBJECTS, ...) OBJECTS
#endif

/* Get selector name (e.g. for KVO use) in a safe way. */
#define M5SelectorName(SELECTOR) (NSStringFromSelector(@selector(SELECTOR)))

#ifndef NS_ASSUME_NONNULL_BEGIN
#define NS_ASSUME_NONNULL_BEGIN _Pragma("clang assume_nonnull begin")
#define NS_ASSUME_NONNULL_END _Pragma("clang assume_nonnull end")
#endif

#if !__has_feature(nullability)
#define nullable
#define nonnull
#define null_unspecified
#define null_resettable
#define __nullable
#define __nonnull
#define __null_unspecified
#endif

#pragma mark Functions

/** Returns whether object/class is subclass of clazz. */
extern BOOL M5ObjectIsOfClass(id object, Class clazz);

/** Returns whether M5ObjectIsOfClass returns true for any of classes. */
extern BOOL M5ObjectIsOfAnyClass(id object, id<NSFastEnumeration> classes);

/** Returns whether M5ObjectIsOfAnyClass returns true for all of objects. */
extern BOOL M5ObjectsAreOfAnyClass(id<NSFastEnumeration> objects, id<NSFastEnumeration> classes);

/** Returns whether property on object equals value. */
extern BOOL M5ObjectPropertyEqualsValue(id object, SEL property, id value);

/** Returns whether M5ObjectPropertyEqualsValue returns true for any of values. */
extern BOOL M5ObjectPropertyEqualsAnyValue(id object, SEL property, id<NSFastEnumeration> values);

/** Returns whether M5ObjectPropertyEqualsAnyValue returns true for all of objects. */
extern BOOL M5ObjectsPropertyEqualsAnyValue(id<NSFastEnumeration> objects, SEL property, id<NSFastEnumeration> values);

/** Returns whether object conforms to all of protocols. */
extern BOOL M5ObjectConformsToProtocols(id object, id<NSFastEnumeration> protocols);

/** Returns whether M5ObjectConformsToProtocols returns true for all of objects. */
extern BOOL M5ObjectsConformToProtocols(id<NSFastEnumeration> objects, id<NSFastEnumeration> protocols);

/** Returns whether protocol includes a method (required or optional) with selector. */
extern BOOL M5ProtocolHasSelector(Protocol *protocol, SEL selector);

/** Returns array of Protocol* the class conforms to. */
extern NSPointerArray* M5ProtocolsForClass(Class clazz);

/** Returns comma-delimitted list of protocol names. */
extern NSString* M5ProtocolsDescription(id<NSFastEnumeration> protocols);

/** Returns comma-delimitted list of class names. */
extern NSString* M5ClassesDescription(id<NSFastEnumeration> classes);

/** Returns comma-delimitted list of object descriptions. */
extern NSString* M5ObjectsDescription(id<NSFastEnumeration> objects);

/** Returns comma-delimitted list of descriptions of properties on objects. */
extern NSString* M5ObjectsPropertyDescription(id<NSFastEnumeration> objects, SEL property);

/** Returns whether object is nil or NULL or NSNull etc. */
extern BOOL M5IsNull(id object);

/** Returns not-M5IsNull. */
extern BOOL M5NotNull(id object);

/** Returns whether M5IsNull or object has a 'count' which is 0, or a 'length' which is 0. */
extern BOOL M5IsEmpty(id object);

/** Returns not-M5IsEmpty. */
extern BOOL M5NotEmpty(id object);

#pragma mark -