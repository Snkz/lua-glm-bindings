#include "luabind.hpp"
class Sample {
    public:
        Sample(std::string script);
        ~Sample();

        void runluafunction(std::string function, glm::vec3 p);
        void getluaglobal(std::string global);
        LuaBind * luaBind;
};

Sample::Sample(std::string script) {
    luaBind = new LuaBind(script);
}

Sample::~Sample() {
    luaBind->clean();
    delete luaBind;
}

void Sample::getluaglobal(std::string global) {
    glm::vec3 p = luaBind->get<glm::vec3>(global);
    std::cout << "C++ recieved " << p.x << ", " << p.y << ", " << p.z << std::endl;
}

void Sample::runluafunction(std::string function, glm::vec3 p) {
    int numargs = 1;
    int numresults = 1;
    glm::vec3 args[numargs] = {p};
    luaBind->execute(function, numargs, args, numresults);
    glm::vec3 r = luaBind->getTop<glm::vec3>();
    std::cout << "C++ recieved " << r.x << ", " << r.y << ", " << r.z << std::endl;

}


int main() {
    Sample *s = new Sample("sample.lua");
    s->getluaglobal("enemy.position");
    s->runluafunction("enemy.wander", glm::vec3(1));

    delete s;
    return 0;
}
