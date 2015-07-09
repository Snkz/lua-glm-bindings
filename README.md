# lua-glm-bindings
Featherweight C++ bindings for lua, for sending and receiving glm vectors


Include the header file in your project to use bindings defined in header

Based on Elias Daler tutorial (specifically his templated getters)


Designed to send and recieve glm::vectors to and from lua, I'm currently using this in my closed source game. I intend to flesh this out more as I flesh it out in game.

Currently can:

    Execute lua functions (all values returned to top of stack)  
        - no param functions X num return for Y types
        - X num of int param functions with X num returns for Y types
        - X num of glm::vec3 param functions with X num returns for Y types

    Retrieve top variable on stack for any type defined in header

    Retrieve any global value (including values defined in tables thanks to Elias Daler
    

None of the errors are handled cause I am a terrible person
