
#include "com_example_myapplication_MainActivity.h"
#include <android/log.h>

#include <iostream>
#include <string>

// other stuff you could include
//#include <android/bitmap.h>
//#include <GLES2/gl2.h>
//#include <GLES2/gl2ext.h>

// setup for logging within NDK
#define  LOG_TAG    "MyApplication"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

typedef JNIEnv JE;

// ------ HELPER FUNCTIONS ----------------------------------------

struct androidbuf : public std::streambuf 
{
    // this class acts as an adapter from cout to Android log
    // using this you can get logging working from your backend
    
    enum { bufsize = 128 }; // ... or some other suitable buffer size
    androidbuf() { this->setp(buffer, buffer + bufsize - 1); }

private:
    
    int overflow(int c)
    {
        if (c == traits_type::eof()) 
        {
            *this->pptr() = traits_type::to_char_type(c);
            this->sbumpc();
        }
        return this->sync()? traits_type::eof(): traits_type::not_eof(c);
    }

    int sync()
    {
        int rc = 0;
        if (this->pbase() != this->pptr()) 
        {
            char writebuf[bufsize+1];
            memcpy(writebuf, this->pbase(), this->pptr() - this->pbase());
            writebuf[this->pptr() - this->pbase()] = '\0';

            rc = __android_log_write(ANDROID_LOG_INFO, LOG_TAG, writebuf) > 0;
            this->setp(buffer, buffer + bufsize - 1);
        }
        return rc;
    }

    char buffer[bufsize];
};

static std::string toString(JE* env, jstring s)
{
    // here's how you convert a Java string into a C++ string.
    const char* cstr = env->GetStringUTFChars(s, 0);
    std::string ss(cstr);
    env->ReleaseStringUTFChars(s, cstr);
    return ss;
}

static jstring fromString(JE* env, const char* s)
{
    // here's how you convert a c++ string into a Java String
    return env->NewStringUTF(s);
}

static bool bufready = false;
static androidbuf abuf;

// ------ INTERFACE FUNCTIONS ----------------------------------------

using std::string;
using std::cout;

static void initlogger()
{
    // install the C++ logger if needed
    if (!bufready)
    {
        // install logger
        bufready = true;
        cout.rdbuf(&abuf);
    }
}

/* native function corresponding to the Java:
 *
 * native String doAStringThing(String instring);
 *
 * in MainActivity.
 */

JNIEXPORT jstring JNICALL Java_com_example_myapplication_MainActivity_doAStringThing (JNIEnv* env, jobject, jstring jinstring)
{
    // note that the function signature has to be EXACTLY right!
    
    // probably wouldnt call this here, but call it somewhere.
    initlogger();

    // so here we take a string passed from java, do something with it
    // and pass a string back.
    // other objects are easy, strings are a bit more tricky. so here
    // we cover the string case.
    
    // get string
    string instring = toString(env, jinstring);

    // emit a log message
    cout << "doAStringThing " << instring << std::endl;

    // do something
    instring += " - from JNI!";

    // return string
    return fromString(env, instring.c_str());
}

