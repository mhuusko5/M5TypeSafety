# M5TypeSafety

Obj-C macros/functions (mostly for debugging) for applying runtime type safety.

## Usage

```obj-c
- (void)exampleMethodTakingSomeObject:(id)object
						   andAString:(NSString *)string
				andAnArrayOfNSNumbers:(NSArray *)numbers
					 andACustomObject:(CustomObject<CustomProtocol, OtherCustomProtocol> *)customObject
						 andACallback:(void(^)())callback
						 andASelector:(SEL)selector
					   andSomePointer:(void *)pointer
			   someCollectionOfThings:(id<NSFastEnumeration>)things {

	// Asserts that object is not nil/NSNull.null
	M5AssertValue(object);

	// Asserts that object is a string or value or date (includes M5AssertValue call)
	M5AssertClass(object, NSString.class, NSValue.class, NSDate.class);


	// Asserts that string really is a string (includes M5AssertValue call)
	M5AssertClass(string, NSString.class);

	// Asserts that string isn't empty (includes M5AssertValue call)
	M5AssertNotEmpty(string);


	// Asserts that numbers is an array (includes M5AssertValue call)
	M5AssertClass(numbers, NSArray.class);

	// Asserts that number is not empty (includes M5AssertValue call)
	M5AssertNotEmpty(numbers);

	// Asserts that numbers contains numbers (includes M5AssertValue call)
	M5AssertContentClass(numbers, NSNumber.class);


	// Asserts that customObject is CustomObject (includes M5AssertValue call)
	M5AssertClass(customObject, CustomObject.class);

	// Asserts that customObject conforms to CustomProtocol and OtherCustomProtocol (includes M5AssertValue call)
	M5AssertProtocol(customObject, @protocol(CustomProtocol), @protocol(OtherCustomProtocol));

	// Asserts that customObject.somePropertyOrMethod equals one of the provided values
	M5AssertProperty(customObject, @selector(somePropertyOrMethod), @"value1", @(2), @"value3");


	// Asserts that callback is not NULL
	M5AssertValue(callback);


	// Asserts that callback is not NULL
	M5AssertCValue(selector);


	// Asserts that callback is not NULL
	M5AssertCValue(pointer);


	M5AssertValue(things);
	M5AssertProtocol(things, @protocol(NSFastEnumeration));
	M5AssertNotEmpty(things);
	M5AssertContentClass(things, NSString.class, NSValue.class, NSDate.class, NSData.class);
	M5AssertContentProtocol(things, @protocol(NSCopying), @protocol(NSCoding), @protocol(NSObject));
	M5AssertContentProperty(things, @selector(someProperty), @"nkwef", @(334), @(3.0), @(YES));
}
```
