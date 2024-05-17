# Java
## Info
Java is a language that was developed to be processor independed and less punishing for common mistakes, thus granting the programmer a more easy way to develop code. This is acomplished by runnnning the code not natively on the processor but in it's own virtual space, the Java Virtual Machine (JVM). As a consequence, errors that would crash most C-Programms are handled as a more soft exception.

### Advantages
- Processor / OS independent
- Good error handeling

### Disadvantages
- Sloppy garbage collector can cause piles of dead data that clogs up the heap.<br>
  A consequence is overall high memory usage and  surges that cause lag.


## Interacting with native code
### Java Native Interface (JNI)
As Java is a managed language, how can we call a unmanaged function?<br>
For this some steps are needed.<br>

1. ) Create a class that contains all wanted function prototypes from a given DLL. The name of the class should be like the Library but does not have to be. Just make sure it it loaded before calling any function to it.
2. ) To load the library Use the function: 
    ```Java 
    System.loadLibrary("MyLibraryName")
    ```
    > Note: The library does not contain a file extension. It will be added internally depending on the system.
3. )  Compile the java code into java class code with:
      ```Java
      javac MyCodeFile.java
      ```
4. ) Create a C-Header from the before compiled Java class with:
       
      Before Java 9
      ```   
      javah -jni MyCompiledJavaCode.class
      ```   
      After Java 9  (javah was deprecated, merged into javac)<br>
      Because javac also compiles the file, step 3 is rendundant
      ```   
      javac MySouceFile.java -h MySourceFile.class
      ```    
5. )
Now we are done with the Java part.<br>
Next step is to implement the now generated function prototypes that we defined in the Java code. Here we implement the use to the C-DLL we intent to use.<br>
Example of a Java function prototype in C:
    ```C
    JNIEXPORT void JNICALL MyJavaNativeFunction(JNIEnv *, jobject);
    ```
As you can see here, every function has two starting parameters. The Java Enviroment and the object this function is attached to, as there are no global functions in Java and always belong to an object.

Additionally, you need to include the Java header files for the C Code aswell.<br>
You can find them whereever you installed java, there is a folder "includes". Copy the path and include it inside your compiler. Binarys are not needed as this DLL will act as an interface and has no direct dependencys to direct Java source code. 


7. ) Compile the finished C-code project with the required java dependencys.

5. ) Done? You can now start the Java program und call the native functions. If everything worked, there should be no problem. If There is an exception, the DLL could be in the wrong directory. It has to be exactly where the jar file is or at a path defined beforehand. 

### Alternatives
A library that uses a smal C-DLL that manages the loading of function pointers. With this you can write all code on the Java end and dont need to create the C-Function wrapping yourself. This works for DLLs but with static librarys the actual C-Wrapping is still required. 
- [Java Native Access (JNA)](https://github.com/java-native-access/jna?tab=readme-ov-file)

---
