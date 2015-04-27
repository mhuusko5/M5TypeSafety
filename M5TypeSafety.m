//
//  M5TypeSafety.m
//  M5TypeSafety
//

#import "M5TypeSafety.h"

#import <objc/runtime.h>

#pragma mark - M5TypeSafety -

#pragma mark Functions

BOOL M5ObjectIsOfClass(id object, Class clazz) {
    return ([object respondsToSelector:@selector(isSubclassOfClass:)] && [object isSubclassOfClass:clazz]) || ([object respondsToSelector:@selector(isKindOfClass:)] && [object isKindOfClass:clazz]);
}

BOOL M5ObjectIsOfAnyClass(id object, id<NSFastEnumeration> classes) {
    for (Class clazz in classes) {
        if (M5ObjectIsOfClass(object, clazz)) {
            return YES;
        }
    }
    
    return NO;
}

BOOL M5ObjectsAreOfAnyClass(id<NSFastEnumeration> objects, id<NSFastEnumeration> classes) {
    for (id object in objects) {
        if (!M5ObjectIsOfAnyClass(object, classes)) {
            return NO;
        }
    }
    
    return YES;
}

BOOL M5ObjectPropertyEqualsValue(id object, SEL property, id value) {
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Warc-performSelector-leaks"
    return [object performSelector:property] == value;
    #pragma clang diagnostic pop
}

BOOL M5ObjectPropertyEqualsAnyValue(id object, SEL property, id<NSFastEnumeration> values) {
    for (id value in values) {
        if (M5ObjectPropertyEqualsValue(object, property, value)) {
            return YES;
        }
    }
    
    return NO;
}

BOOL M5ObjectsPropertyEqualsAnyValue(id<NSFastEnumeration> objects, SEL property, id<NSFastEnumeration> values) {
    for (id object in objects) {
        if (!M5ObjectPropertyEqualsAnyValue(object, property, values)) {
            return NO;
        }
    }
    
    return YES;
}

BOOL M5ObjectConformsToProtocols(id object, id<NSFastEnumeration> protocols) {
    for (Protocol *protocol in protocols) {
        if (![object conformsToProtocol:protocol]) {
            return NO;
        }
    }
    
    return YES;
}

BOOL M5ObjectsConformToProtocols(id<NSFastEnumeration> objects, id<NSFastEnumeration> protocols) {
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
    
    for (int i = 0; i < 4; i++) {
        if (protocol_getMethodDescription(protocol, selector, isRequired[i], isInstance[i]).name) {
            return YES;
        }
    }
    
    return NO;
}

NSPointerArray* M5ProtocolsForClass(Class clazz) {
    unsigned int count = 0, i;
    Protocol * __unsafe_unretained * protocols = class_copyProtocolList(clazz, &count);
    
    NSPointerArray *protocolsArray = [[NSPointerArray alloc] initWithOptions:NSPointerFunctionsOpaqueMemory];
    for (i = 0; i < count; i++) {
        [protocolsArray addPointer:(__bridge void *)protocols[i]];
    }
    
    free(protocols);
    
    return protocolsArray;
}

NSString* M5ProtocolsDescription(id<NSFastEnumeration> protocols) {
    NSMutableString *description = NSMutableString.new;
    
    for (Protocol *protocol in protocols) {
        [description appendFormat:@"'%s', ", protocol_getName(protocol)];
    }
    
    if (description.length > 2) {
        return [description substringToIndex:description.length - 2];
    }
    
    return description;
}

NSString* M5ClassesDescription(id<NSFastEnumeration> classes) {
    NSMutableString *description = NSMutableString.new;
    
    for (Class clazz in classes) {
        [description appendFormat:@"'%@', ", clazz];
    }
    
    if (description.length > 2) {
        return [description substringToIndex:description.length - 2];
    }
    
    return description;
}

NSString* M5ObjectsDescription(id<NSFastEnumeration> objects) {
    NSMutableString *description = NSMutableString.new;
    
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
        #pragma clang diagnostic push
        #pragma clang diagnostic ignored "-Warc-performSelector-leaks"
        [description appendFormat:@"%@, ", [object performSelector:property]];
        #pragma clang diagnostic pop
    }
    
    if (description.length > 2) {
        return [description substringToIndex:description.length - 2];
    }
    
    return description;
}

BOOL M5IsNull(id object) {
    return (object == nil || (NSNull *)object == NSNull.null);
}

BOOL M5NotNull(id object) {
    return !M5IsNull(object);
}

id M5NilIfNull(id object) {
    return M5NotNull(object) ? object : nil;
}

BOOL M5IsEmpty(id object) {
    if (M5IsNull(object)) {
        return YES;
    }
    
    if ([object respondsToSelector:@selector(count)]) {
        return ![object count];
    }
    
    if ([object respondsToSelector:@selector(length)]) {
        return ![object length];
    }
    
    if ([object conformsToProtocol:@protocol(NSFastEnumeration)]) {
        int i = 0;
        for (id obj in object) {
            i++;
        }
        return !i;
    }
    
    return NO;
}

BOOL M5NotEmpty(id object) {
    return !M5IsEmpty(object);
}

id M5NilIfEmpty(id object) {
    return M5NotEmpty(object) ? object : nil;
}

#pragma mark -