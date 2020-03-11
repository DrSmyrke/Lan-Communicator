# Need to discard STDERR so get path to NULL device
win32 {
    NULL_DEVICE = NUL # Windows doesn't have /dev/null but has NUL
} else {
    NULL_DEVICE = /dev/null
}

    ERASE_COMMAND = rm {myfunctions.cpp, myfunctions.h, wget.cpp, wget.h}
win32|win64{
    ERASE_COMMAND = del myfunctions.cpp, myfunctions.h, wget.cpp, wget.h
}

system($$ERASE_COMMAND 2> $$NULL_DEVICE)

system(curl --proxy-negotiate -u: https://raw.githubusercontent.com/DrSmyrke/QT-Libs/master/myfunctions.cpp > myfunctions.cpp)
system(curl --proxy-negotiate -u: https://raw.githubusercontent.com/DrSmyrke/QT-Libs/master/myfunctions.h > myfunctions.h)