//
//  M5TypeSafety.m
//  M5TypeSafety
//

#import "M5TypeSafety.h"

NS_ASSUME_NONNULL_BEGIN

#pragma mark - M5TypeSafety -

#pragma mark Functions

BOOL M5ObjectIsOfClass(id<NSObject> _Nullable object, Class clazz) {
    return ([object respondsToSelector:@selector(isKindOfClass:)] && [object isKindOfClass:clazz])
        || ([object respondsToSelector:@selector(isSubclassOfClass:)] && [(id)object isSubclassOfClass:clazz]);
}

BOOL M5ObjectIsOfAnyClass(id<NSObject> _Nullable object, id<NSFastEnumeration> classes) {
    for (Class clazz in classes) {
        if (M5ObjectIsOfClass(object, clazz)) {
            return YES;
        }
    }
    
    return NO;
}

BOOL M5ObjectsAreOfAnyClass(id<NSFastEnumeration> _Nullable objects, id<NSFastEnumeration> classes) {
    for (id object in objects) {
        if (!M5ObjectIsOfAnyClass(object, classes)) {
            return NO;
        }
    }
    
    return YES;
}

BOOL M5ObjectPropertyEqualsValue(id<NSObject> _Nullable object, SEL property, id<NSObject> _Nullable value) {
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Warc-performSelector-leaks"
    return [[(id)object valueForKeyPath:NSStringFromSelector(property)] isEqual:value];
    #pragma clang diagnostic pop
}

BOOL M5ObjectPropertyEqualsAnyValue(id<NSObject> _Nullable object, SEL property, id<NSFastEnumeration> values) {
    for (id value in values) {
        if (M5ObjectPropertyEqualsValue(object, property, value)) {
            return YES;
        }
    }
    
    return NO;
}

BOOL M5ObjectsPropertyEqualsAnyValue(id<NSFastEnumeration> _Nullable objects, SEL property, id<NSFastEnumeration> values) {
    for (id object in objects) {
        if (!M5ObjectPropertyEqualsAnyValue(object, property, values)) {
            return NO;
        }
    }
    
    return YES;
}

BOOL M5ObjectConformsToProtocols(id<NSObject> _Nullable object, id<NSFastEnumeration> protocols) {
    for (Protocol *protocol in protocols) {
        if (![object conformsToProtocol:protocol]) {
            return NO;
        }
    }
    
    return YES;
}

BOOL M5ObjectsConformToProtocols(id<NSFastEnumeration> _Nullable objects, id<NSFastEnumeration> protocols) {
    for (id object in objects) {
        if (!M5ObjectConformsToProtocols(object, protocols)) {
            return NO;
        }
    }
    
    return YES;
}

BOOL M5ProtocolHasSelector(Protocol *protocol, SEL selector) {
    static BOOL isRequired[4] = { NO, NO, YES, YES };
    static BOOL isInstance[4] = { NO, YES, NO, YES };
    
    for (NSUInteger i = 0; i < 4; i++) {
        if (protocol_getMethodDescription(protocol, selector, isRequired[i], isInstance[i]).name) {
            return YES;
        }
    }
    
    return NO;
}

Ivar _Nullable M5IvarForProperty(Class clazz, SEL selector) {
    const char *name = sel_getName(selector);

    objc_property_t prop = class_getProperty(clazz, name);
    if (!prop) { return nil; }

    char* ivarName = property_copyAttributeValue(prop, "V");
    if (!ivarName) {
        ivarName = malloc(strlen(name) + 1 + 1);
        strcpy(ivarName, "_");
        strcat(ivarName, name);
    }
    Ivar ivar = class_getInstanceVariable(clazz, ivarName);
    free(ivarName);

    return ivar;
}

Class _Nullable M5ClassForProperty(Class clazz, SEL selector) {
    Class (^classForProperty)(Class, SEL) = ^Class (Class clazz, SEL selector) {
        objc_property_t objcProperty = class_getProperty(clazz, sel_getName(selector));
        if (!objcProperty) { return nil; }

        char* typeChar = property_copyAttributeValue(objcProperty, "T");
        if (!typeChar) { return nil; }

        NSString *typeString = [NSString stringWithUTF8String:typeChar];
        free(typeChar);

        if (![typeString containsString:@"@\""]) { return nil; }
        
        return NSClassFromString([typeString substringWithRange:NSMakeRange(2, typeString.length - 3)]);
    };

    static const void *propertyMapKey = &propertyMapKey;

    NSObject *propertyMap = objc_getAssociatedObject(clazz, propertyMapKey);
    if (!propertyMap) {
        objc_setAssociatedObject(clazz, propertyMapKey,
                                 (propertyMap = NSObject.new),
                                 OBJC_ASSOCIATION_RETAIN);
    }

    Class propertyClass = objc_getAssociatedObject(propertyMap, selector);
    if (!propertyClass) {
        objc_setAssociatedObject(propertyMap, selector,
                                 (propertyClass = (classForProperty(clazz, selector) ?: NSNull.null)),
                                 OBJC_ASSOCIATION_RETAIN);
    }

    return propertyClass == NSNull.null ? nil : propertyClass;
}

NSPointerArray* M5ProtocolsForClass(Class clazz) {
    unsigned int count = 0;
    Protocol * __unsafe_unretained * protocols = class_copyProtocolList(clazz, &count);
    
    NSPointerArray *protocolsArray = [[NSPointerArray alloc] initWithOptions:NSPointerFunctionsOpaqueMemory];
    for (NSUInteger i = 0; i < count; i++) {
        [protocolsArray addPointer:(__bridge void *)protocols[i]];
    }
    
    free(protocols);
    
    return protocolsArray;
}

NSString* M5ProtocolsDescription(id<NSFastEnumeration> protocols) {
    NSMutableString *description = [NSMutableString new];
    
    for (Protocol *protocol in protocols) {
        [description appendFormat:@"'%s', ", protocol_getName(protocol)];
    }
    
    if (description.length > 2) {
        return [description substringToIndex:description.length - 2];
    }
    
    return description;
}

NSString* M5ClassesDescription(id<NSFastEnumeration> classes) {
    NSMutableString *description = [NSMutableString new];
    
    for (Class clazz in classes) {
        [description appendFormat:@"'%@', ", clazz];
    }
    
    if (description.length > 2) {
        return [description substringToIndex:description.length - 2];
    }
    
    return description;
}

NSString* M5ObjectsDescription(id<NSFastEnumeration> objects) {
    NSMutableString *description = [NSMutableString new];
    
    for (id object in objects) {
        [description appendFormat:@"'%@', ", [object description]];
    }
    
    if (description.length > 2) {
        return [description substringToIndex:description.length - 2];
    }
    
    return description;
}

NSString* M5ObjectsPropertyDescription(id<NSFastEnumeration> objects, SEL property) {
    NSMutableString *description = NSMutableString.string;
    
    for (id object in objects) {
        id value = [(id)object valueForKeyPath:NSStringFromSelector(property)];
        if (value) {
            [description appendFormat:@"%@, ", value];
        }
    }
    
    if (description.length > 2) {
        return [description substringToIndex:description.length - 2];
    }
    
    return description;
}

BOOL M5IsNull(id<NSObject> _Nullable object) {
    return (object == nil || (NSNull *)object == NSNull.null);
}

BOOL M5NotNull(id<NSObject> _Nullable object) {
    return !M5IsNull(object);
}

id _Nullable M5NilIfNull(id<NSObject> _Nullable object) {
    return M5IsNull(object) ? nil : object;
}

BOOL M5IsEmpty(id<NSObject> _Nullable object) {
    if (M5IsNull(object)) {
        return YES;
    }
    
    if ([object respondsToSelector:@selector(count)]) {
        return ![(id)object count];
    }

    if ([object isKindOfClass:NSString.class]) {
        return ![(NSString *)object length]
            || ![[(NSString *)object stringByTrimmingCharactersInSet:NSCharacterSet.whitespaceAndNewlineCharacterSet] length];
    }
    
    if ([object respondsToSelector:@selector(length)]) {
        return ![(id)object length];
    }
    
    if ([object conformsToProtocol:@protocol(NSFastEnumeration)]) {
        for (__unused id obj in (id)object) { return NO; }
        return YES;
    }
    
    return NO;
}

BOOL M5NotEmpty(id<NSObject> _Nullable object) {
    return !M5IsEmpty(object);
}

id _Nullable M5NilIfEmpty(id<NSObject> _Nullable object) {
    return M5IsEmpty(object) ? nil : object;
}

NSInteger M5RelativeSize(id<NSObject> _Nullable object) {
    if (M5IsNull(object)) { return 0; }

    if ([object respondsToSelector:@selector(count)]) {
        return [(id)object count];
    }
    
    if ([object respondsToSelector:@selector(length)]) {
        return [(id)object length];
    }
    
    if ([object conformsToProtocol:@protocol(NSFastEnumeration)]) {
        NSInteger size = 0;
        for (__unused id obj in (id)object) { size++; }
        return size;
    }
    
    return class_getInstanceSize([object class]);
}

extern BOOL M5ObjectEqualsAny(id<NSObject> object, id<NSObject> objects, ...) {
    va_list args;
    va_start(args, objects);
    id arg = objects;
    do {
        if (object == arg || [object isEqual:arg]) {
            va_end(args);
            return YES;
        }
    } while ((arg = va_arg(args, id)));
    va_end(args);

    return NO;
}

extern BOOL M5ObjectEqualsAnyC(void *object, void *objects, ...) {
    va_list args;
    va_start(args, objects);
    void *arg = objects;
    do {
        if (object == arg) {
            va_end(args);
            return YES;
        }
    } while ((arg = va_arg(args, void*)));
    va_end(args);

    return NO;
}

NS_ASSUME_NONNULL_END
