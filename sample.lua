print("Hello World!")
require "math"
enemy = {
    position = {
        x = 200.0,
        y = 300.0,
        z = 400.0,
    },
}

function enemy.wander (player) 
    print("lua recieved", player["x"], player["y"], player["z"]);
    x = math.random(enemy["position"]["x"] - 100, enemy["position"]["x"] + 100)
    y = math.random(enemy["position"]["y"] - 100, enemy["position"]["y"] + 100)
    z = math.random(enemy["position"]["z"] - 100, enemy["position"]["z"] + 100)

    enemy["position"]["x"] = x
    enemy["position"]["y"] = y
    enemy["position"]["z"] = z

    return enemy["position"]
end


