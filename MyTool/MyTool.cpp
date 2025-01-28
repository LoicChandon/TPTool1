#include <iostream>
#include <fstream>
#include <string>
#include "json.hpp"

using json = nlohmann::json;

void ShowInfos(const std::string& filePath) {
    // Vérifier si le fichier existe
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Le fichier '" << filePath << "' n'existe pas ou ne peut pas être ouvert." << std::endl;
        return;
    }

    // Charger le contenu du fichier JSON
    json uproject;
    try {
        file >> uproject;
    } catch (const std::exception& e) {
        std::cerr << "Erreur lors de la lecture du fichier JSON : " << e.what() << std::endl;
        return;
    }

    // Afficher le nom du jeu (nom du fichier sans extension)
    size_t lastSlash = filePath.find_last_of("/\\");
    std::string gameName = filePath.substr(lastSlash + 1, filePath.find_last_of('.') - lastSlash - 1);
    std::cout << "Nom du jeu : " << gameName << std::endl;

    // Afficher la version de Unreal Engine utilisée
    if (uproject.contains("EngineAssociation")) {
        std::cout << "Version Unreal : " << uproject["EngineAssociation"].get<std::string>() << std::endl;
    } else {
        std::cout << "Version Unreal : Non spécifiée" << std::endl;
    }

    // Vérifier si le projet est "From Source"
    if (uproject.contains("EngineAssociation") &&
        uproject["EngineAssociation"].get<std::string>().find("Source") != std::string::npos) {
        std::cout << "From Source : Oui" << std::endl;
    } else {
        std::cout << "From Source : Non" << std::endl;
    }

    // Afficher les plugins utilisés
    if (uproject.contains("Plugins")) {
        std::cout << "Plugins utilisés :" << std::endl;
        for (const auto& plugin : uproject["Plugins"]) {
            std::cout << "- " << plugin["Name"].get<std::string>();
            if (plugin.contains("Enabled") && plugin["Enabled"].get<bool>()) {
                std::cout << " (Enabled)";
            }
            std::cout << std::endl;
        }
    } else {
        std::cout << "Aucun plugin trouvé." << std::endl;
    }
}

int main(int argc, char* argv[]) {
    // Vérifier les arguments
    if (argc < 2) {
        std::cerr << "Usage : TPToolingCpp <CHEMIN_DU_FICHIER_UPROJECT>" << std::endl;
        return 1;
    }

    std::string filePath = argv[1];
    ShowInfos(filePath);

    return 0;
}
