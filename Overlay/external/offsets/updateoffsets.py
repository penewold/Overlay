import requests
BASE = "https://raw.githubusercontent.com/a2x/cs2-dumper/refs/heads/main/output/"

filenames = ["animationsystem_dll.hpp", "buttons.hpp", "client_dll.hpp", "engine2_dll.hpp", "host_dll.hpp", "interfaces.hpp", "materialsystem2_dll.hpp", "networksystem_dll.hpp", "offsets.hpp", "panorama_dll.hpp", "particles_dll.hpp", "pulse_system_dll.hpp", "rendersystemdx11_dll.hpp", "resourcesystem_dll.hpp", "scenesystem_dll.hpp", "schemasystem_dll.hpp", "server_dll.hpp", "soundsystem_dll.hpp", "vphysics2_dll.hpp", "worldrenderer_dll.hpp"]

for filename in filenames:
    response = requests.get(BASE + filename)
    if response.status_code == 200:
        with open(filename, "w") as file:
            file.write(response.content.decode())
        print(filename + ": OK")
    else:
        print(filename + ": NOT OK")