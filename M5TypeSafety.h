//
//  M5TypeSafety.h
//  M5TypeSafety
//

#import <Foundation/Foundation.h>

#pragma mark - M5TypeSafety -

#pragma mark Macros

#define _M5MacroStringify(K...) (@#K)

#define _M5Assert(CONDITION, MESSAGE) \
({ \
    if (!(CONDITION)) { \
        NSLog(@"%@", MESSAGE); \
        [NSException raise:@"M5Assertion" format:MESSAGE]; \
    } \
})

#ifdef DEBUG
/* Asserts with case as message of assertion. */
#define M5Assert(CASE) \
({ \
    NSString *M5AssertMessage = [NSString stringWithFormat:@"M5Assert: %@ should be true.", _M5MacroStringify(CASE)]; \
    _M5Assert(CASE, M5AssertMessage); \
})

/* Asserts that object is not nil or NSNull. Returns object. */
#define M5AssertValue(OBJECT) \
({ \
    id M5AssertValue = OBJECT; \
    NSString *M5AssertValueMessage = [NSString stringWithFormat:@"M5AssertValue: '%@' can't be nil or NSNull.", _M5MacroStringify(OBJECT)]; \
    _M5Assert(M5NotNull(M5AssertValue), M5AssertValueMessage); \
    M5AssertValue; \
})

/* Asserts that object (string, array, dictionary, etc.) is not nil/NSNull/empty. Returns object. */
#define M5AssertNotEmpty(OBJECT) \
({ \
    id M5AssertNotEmpty = OBJECT; \
    NSString *M5AssertNotEmptyMessage = [NSString stringWithFormat:@"M5AssertNotEmpty: '%@' can't be null/empty.", _M5MacroStringify(OBJECT)]; \
    _M5Assert(M5NotEmpty(M5AssertNotEmpty), M5AssertNotEmptyMessage); \
    M5AssertNotEmpty; \
})

/* Asserts that object is not empty, if it is not null. Returns object. */
#define M5AssertNotEmptyIfValue(OBJECT) \
({ \
    id M5AssertNotEmptyIfValue = OBJECT; \
    if (M5AssertNotEmptyIfValue) { \
        NSString *M5AssertNotEmptyIfValueMessage = [NSString stringWithFormat:@"M5AssertNotEmptyIfValue: '%@' can't be empty.", _M5MacroStringify(OBJECT)]; \
        _M5Assert(M5NotEmpty(M5AssertNotEmptyIfValue), M5AssertNotEmptyIfValueMessage); \
    } \
    M5AssertNotEmptyIfValue; \
})

/* Asserts that pointer is not NULL. Returns pointer. */
#define M5AssertCValue(OBJECT) \
({ \
    void *M5AssertCValue = OBJECT; \
    NSString *M5AssertCValueMessage = [NSString stringWithFormat:@"M5AssertCValue: '%@' can't be NULL.", _M5MacroStringify(OBJECT)]; \
    _M5Assert(M5AssertCValue != NULL, M5AssertCValueMessage); \
    M5AssertCValue; \
})

/* Asserts that object is not nil/NSNull, and is subclass of one of the provided classes. Returns object. */
#define M5AssertClass(OBJECT, ...) \
({ \
    id M5AssertClass = OBJECT; \
    NSString *M5AssertClassMessage = [NSString stringWithFormat:@"M5AssertClass: '%@' must (not be null) be one of: %@", _M5MacroStringify(OBJECT), M5ClassesDescription(@[__VA_ARGS__])]; \
    _M5Assert(M5NotNull(M5AssertClass) && M5ObjectIsOfAnyClass(M5AssertClass, @[__VA_ARGS__]), M5AssertClassMessage); \
    M5AssertClass; \
})

/* Asserts that object is subclass of one of the provided classes if it is not null. Returns object. */
#define M5AssertClassIfValue(OBJECT, ...) \
({ \
    id M5AssertClassIfValue = OBJECT; \
    if (M5AssertClassIfValue) { \
        NSString *M5AssertClassIfValueMessage = [NSString stringWithFormat:@"M5AssertClassIfValue: '%@' must be one of: %@", _M5MacroStringify(OBJECT), M5ClassesDescription(@[__VA_ARGS__])]; \
        _M5Assert(M5NotNull(M5AssertClassIfValue) && M5ObjectIsOfAnyClass(M5AssertClassIfValue, @[__VA_ARGS__]), M5AssertClassIfValueMessage); \
    } \
    M5AssertClassIfValue; \
})

/* Asserts that property on object (and that object is not nil/NSNull) equals one of the provided values. Returns object. */
#define M5AssertProperty(OBJECT, PROPERTY, ...) \
({ \
    id M5AssertProperty = OBJECT; \
    NSString *M5AssertPropertyMessage = [NSString stringWithFormat:@"M5AssertProperty: '%@' on '%@' (which must not be null) must equal one of: %@", NSStringFromSelector(PROPERTY), _M5MacroStringify(OBJECT), M5ObjectsDescription(@[__VA_ARGS__])]; \
    _M5Assert(M5NotNull(M5AssertProperty) && M5ObjectPropertyEqualsAnyValue(M5AssertProperty, PROPERTY, @[__VA_ARGS__]), M5AssertPropertyMessage); \
    M5AssertProperty; \
})

/* Asserts that property on object (if the object is not null) equals one of the provided values. Returns object. */
#define M5AssertPropertyIfValue(OBJECT, PROPERTY, ...) \
({ \
    id M5AssertPropertyIfValue = OBJECT; \
    if (M5AssertPropertyIfValue) { \
        NSString *M5AssertPropertyIfValueMessage = [NSString stringWithFormat:@"M5AssertPropertyIfValue: '%@' on '%@' must equal one of: %@", NSStringFromSelector(PROPERTY), _M5MacroStringify(OBJECT), M5ObjectsDescription(@[__VA_ARGS__])]; \
        _M5Assert(M5NotNull(M5AssertPropertyIfValue) && M5ObjectPropertyEqualsAnyValue(M5AssertPropertyIfValue, PROPERTY, @[__VA_ARGS__]), M5AssertPropertyIfValueMessage); \
    } \
    M5AssertPropertyIfValue; \
})

/* Asserts that object is not nil/NSNull and conforms to all of the provided protocols. Returns object. */
#define M5AssertProtocol(OBJECT, ...) \
({ \
    id M5AssertProtocol = OBJECT; \
    NSString *M5AssertProtocolMessage = [NSString stringWithFormat:@"M5AssertProtocol: '%@' must (not be null) conform to all of: %@", _M5MacroStringify(OBJECT), M5ProtocolsDescription(@[__VA_ARGS__])]; \
    _M5Assert(M5NotNull(M5AssertProtocol) && M5ObjectConformsToProtocols(M5AssertProtocol, @[__VA_ARGS__]), M5AssertProtocolMessage); \
    M5AssertProtocol; \
})

/* Asserts that object (if not nil/NSNull) conforms to all of the provided protocols. Returns object. */
#define M5AssertProtocolIfValue(OBJECT, ...) \
({ \
    id M5AssertProtocolIfValue = OBJECT; \
    if (M5AssertProtocolIfValue) { \
        NSString *M5AssertProtocolIfValueMessage = [NSString stringWithFormat:@"M5AssertProtocolIfValue: '%@' must conform to all of: %@", _M5MacroStringify(OBJECT), M5ProtocolsDescription(@[__VA_ARGS__])]; \
        _M5Assert(M5NotNull(M5AssertProtocolIfValue) && M5ObjectConformsToProtocols(M5AssertProtocolIfValue, @[__VA_ARGS__]), M5AssertProtocolIfValueMessage); \
    } \
    M5AssertProtocolIfValue; \
})

/* Asserts that the (not empty) objects are each subclasses of one of the provided classes, returns object. */
#define M5AssertContentClass(OBJECTS, ...) \
({ \
    id<NSFastEnumeration> M5AssertContentClass = OBJECTS; \
    NSString *M5AssertContentClassMessage = [NSString stringWithFormat:@"M5AssertContentClass: Contents of (not empty!) '%@' must be one of: %@", _M5MacroStringify(OBJECTS), M5ClassesDescription(@[__VA_ARGS__])]; \
    _M5Assert(M5NotEmpty(M5AssertContentClass) && M5ObjectsAreOfAnyClass(M5AssertContentClass, @[__VA_ARGS__]), M5AssertContentClassMessage); \
    M5AssertContentClass; \
})

/* Asserts that the objects (if any) are each subclasses of one of the provided classes, returns object. */
#define M5AssertContentClassIfValue(OBJECTS, ...) \
({ \
    id<NSFastEnumeration> M5AssertContentClassIfValue = OBJECTS; \
    if (M5NotEmpty(M5AssertContentClassIfValue)) { \
        NSString *M5AssertContentClassIfValueMessage = [NSString stringWithFormat:@"M5AssertContentClassIfValue: Contents of '%@' must be one of: %@", _M5MacroStringify(OBJECTS), M5ClassesDescription(@[__VA_ARGS__])]; \
        _M5Assert(M5ObjectsAreOfAnyClass(M5AssertContentClassIfValue, @[__VA_ARGS__]), M5AssertContentClassIfValueMessage); \
    } \
    M5AssertContentClassIfValue; \
})

/* Asserts that the property on each of the (not empty) objects equals one of the provided values. */
#define M5AssertContentProperty(OBJECTS, PROPERTY, ...) \
({ \
    id<NSFastEnumeration> M5AssertContentProperty = OBJECTS; \
    NSString *M5AssertContentPropertyMessage = [NSString stringWithFormat:@"M5AssertContentProperty: '%@' on contents of (not empty!) '%@' must equal one of: %@", NSStringFromSelector(PROPERTY), _M5MacroStringify(OBJECT), M5ObjectsDescription(@[__VA_ARGS__])]; \
    _M5Assert(M5NotEmpty(M5AssertContentProperty) && M5ObjectsPropertyEqualsAnyValue(M5AssertContentProperty, PROPERTY, @[__VA_ARGS__]), M5AssertContentPropertyMessage); \
    M5AssertContentProperty; \
})

/* Asserts that the property on each of the objects (if any) equals one of the provided values. */
#define M5AssertContentPropertyIfValue(OBJECTS, PROPERTY, ...) \
({ \
    id<NSFastEnumeration> M5AssertContentPropertyIfValue = OBJECTS; \
    if (M5NotEmpty(M5AssertContentPropertyIfValue)) { \
        NSString *M5AssertContentPropertyIfValueMessage = [NSString stringWithFormat:@"M5AssertContentPropertyIfValue: '%@' on contents of '%@' must equal one of: %@", NSStringFromSelector(PROPERTY), _M5MacroStringify(OBJECT), M5ObjectsDescription(@[__VA_ARGS__])]; \
        _M5Assert(M5ObjectsPropertyEqualsAnyValue(M5AssertContentPropertyIfValue, PROPERTY, @[__VA_ARGS__]), M5AssertContentPropertyIfValueMessage); \
    } \
    M5AssertContentPropertyIfValue; \
})

/* Asserts that each of the (not empty) objects conforms to all of the provided protocols. */
#define M5AssertContentProtocol(OBJECTS, ...) \
({ \
    id<NSFastEnumeration> M5AssertContentProtocol = OBJECTS; \
    NSString *M5AssertContentProtocolMessage = [NSString stringWithFormat:@"M5AssertContentProtocol: Contents of (not empty!) '%@' must conform to all of: %@", _M5MacroStringify(OBJECTS), M5ProtocolsDescription(@[__VA_ARGS__])]; \
    _M5Assert(M5NotEmpty(M5AssertContentProtocol) && M5ObjectsConformToProtocols(M5AssertContentProtocol, @[__VA_ARGS__]), M5AssertContentProtocolMessage); \
    M5AssertContentProtocol; \
})

/* Asserts that each of the objects (if there are any) conforms to all of the provided protocols. */
#define M5AssertContentProtocolIfValue(OBJECTS, ...) \
({ \
    id<NSFastEnumeration> M5AssertContentProtocolIfValue = OBJECTS; \
    if (M5NotEmpty(M5AssertContentProtocolIfValue)) { \
        NSString *M5AssertContentProtocolIfValueMessage = [NSString stringWithFormat:@"M5AssertContentProtocolIfValue: Contents of '%@' must conform to all of: %@", _M5MacroStringify(OBJECTS), M5ProtocolsDescription(@[__VA_ARGS__])]; \
        _M5Assert(M5ObjectsConformToProtocols(M5AssertContentProtocolIfValue, @[__VA_ARGS__]), M5AssertContentProtocolIfValueMessage); \
    } \
    M5AssertContentProtocolIfValue; \
})
#else
#define M5Assert(CASE)
#define M5AssertValue(OBJECT) ({ OBJECT; })
#define M5AssertNotEmpty(OBJECT) ({ OBJECT; })
#define M5AssertNotEmptyIfValue(OBJECT) ({ OBJECT; })
#define M5AssertCValue(OBJECT) ({ OBJECT; })
#define M5AssertClass(OBJECT, ...) ({ OBJECT; })
#define M5AssertClassIfValue(OBJECT, ...) ({ OBJECT; })
#define M5AssertProperty(OBJECT, PROPERTY, ...) ({ OBJECT; })
#define M5AssertPropertyIfValue(OBJECT, PROPERTY, ...) ({ OBJECT; })
#define M5AssertProtocol(OBJECT, ...) ({ OBJECT; })
#define M5AssertProtocolIfValue(OBJECT, ...) ({ OBJECT; })
#define M5AssertContentClass(OBJECTS, ...) ({ OBJECTS; })
#define M5AssertContentClassIfValue(OBJECTS, ...) ({ OBJECTS; })
#define M5AssertContentProperty(OBJECTS, PROPERTY, ...) ({ OBJECTS; })
#define M5AssertContentPropertyIfValue(OBJECTS, PROPERTY, ...) ({ OBJECTS; })
#define M5AssertContentProtocol(OBJECTS, ...) ({ OBJECTS; })
#define M5AssertContentProtocolIfValue(OBJECTS, ...) ({ OBJECTS; })
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

/** Returns object if M5NotNull, else nil. */
extern id M5NilIfNull(id object);

/** Returns whether M5IsNull or object has a 'count' which is 0, or a 'length' which is 0. */
extern BOOL M5IsEmpty(id object);

/** Returns not-M5IsEmpty. */
extern BOOL M5NotEmpty(id object);

/** Returns object if M5NotEmpty, else nil. */
extern id M5NilIfEmpty(id object);

#pragma mark -