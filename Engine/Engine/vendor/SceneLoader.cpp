#include <iostream>
#include <iterator>
#include <algorithm>
#include <stdio.h>

#include "SceneLoader.h"
#include "tiny_obj_loader.h"

#include "../Math/Math.h"

#pragma warning(disable:4996)

namespace Rendering
{
    static const int s_MAX_LINE_LENGTH = 2048;
    FILE* filepoint;
    bool LoadSceneFromFile(const std::string& filename, Scene* scene, RenderOptions& renderOptions)
    {
        std::string rootPath = filename.substr(0, filename.find_last_of("/\\")) + "/";

        FILE* file = fopen(filename.c_str(), "r");
        if (!file)
        {
            printf("Couldn't open %s for reading\n", filename.c_str());
            return false;
        }

        printf("Loading Scene..\n");

        struct MaterialData
        {
            Material mat;
            int id;
        };

        std::map<std::string, MaterialData> materialMap;

        std::vector<std::string> albedoTex;
        std::vector<std::string> metallicRoughnessTex;
        std::vector<std::string> normalTex;

        char line[s_MAX_LINE_LENGTH];

        // defaultMat
        Material defaultMat;
        scene->AddMaterial(defaultMat);

        bool cameraAdded = false;

        while (fgets(line, s_MAX_LINE_LENGTH, file))
        {
            // skip comments
            if (line[0] == '#') {
                continue;
            }

            // name used for materials and meshes
            char name[s_MAX_LINE_LENGTH] = { 0 };

            //--------------------------------------------
            // Material
            if (scanf_s (line, " material %s", name) == 1)
            {
                Material material;
                char albedoTexName[100] = "None";
                char paramsTexName[100] = "None"; // metallic and roughness
                char normalTexName[100] = "None";

                while (fgets(line, s_MAX_LINE_LENGTH, file))
                {
                    // end group
                    if (strchr(line, '}')) {
                        break;
                    }

                    scanf_s (line, " name %s", name);
                    scanf_s (line, " color %f %f %f", &material.albedo.x, &material.albedo.y, &material.albedo.z);
                    scanf_s (line, " emission %f %f %f", &material.emission.x, &material.emission.y, &material.emission.z);
                    scanf_s (line, " materialType %f", &material.type);
                    scanf_s (line, " metallic %f", &material.metallic);
                    scanf_s (line, " roughness %f", &material.roughness);
                    scanf_s (line, " ior %f", &material.ior);
                    scanf_s (line, " transmittance %f", &material.transmittance);

                    scanf_s (line, " albedoTexture %s", albedoTexName);
                    scanf_s (line, " metallicRoughnessTexture %s", paramsTexName);
                    scanf_s (line, " normalTexture %s", normalTexName);
                }

                // Albedo Texture
                if (strcmp(albedoTexName, "None") != 0) {
                    material.albedoTexID = scene->AddTexture(rootPath + albedoTexName);
                }

                // MetallicRoughness Texture
                if (strcmp(paramsTexName, "None") != 0) {
                    material.paramsTexID = scene->AddTexture(rootPath + paramsTexName);
                }

                // Normal Map Texture
                if (strcmp(normalTexName, "None") != 0) {
                    material.normalmapTexID = scene->AddTexture(rootPath + normalTexName);
                }

                // add material to map
                if (materialMap.find(name) == materialMap.end()) // New material
                {
                    int id = scene->AddMaterial(material);
                    materialMap[name] = MaterialData{ material, id };
                }
            }

            //--------------------------------------------
            // Light
            if (strstr(line, "light"))
            {
                Light light;
                Vector3 v1, v2;
                char lightType[20] = "None";

                while (fgets(line, s_MAX_LINE_LENGTH, file))
                {
                    // end group
                    if (strchr(line, '}')) {
                        break;
                    }

                    scanf_s (line, " position %f %f %f", &light.position.x, &light.position.y, &light.position.z);
                    scanf_s(line, " emission %f %f %f", &light.emission.x, &light.emission.y, &light.emission.z);

                    scanf_s (line, " radius %f", &light.radius);
                    scanf_s (line, " v1 %f %f %f", &v1.x, &v1.y, &v1.z);
                    scanf_s (line, " v2 %f %f %f", &v2.x, &v2.y, &v2.z);
                    scanf_s (line, " type %s", lightType);
                }

                if (strcmp(lightType, "Quad") == 0)
                {
                    light.type = LightType::QuadLight;
                    light.u = v1 - light.position;
                    light.v = v2 - light.position;
                    light.area = Vector3::CrossProduct(light.u, light.v).Size();
                }
                else if (strcmp(lightType, "Sphere") == 0)
                {
                    light.type = LightType::SphereLight;
                    light.area = 4.0f * PI * light.radius * light.radius;
                }

                scene->AddLight(light);
            }

            //--------------------------------------------
            // Camera
            if (strstr(line, "Camera"))
            {
                Vector3 position;
                Vector3 lookAt;

                float fov;
                float aperture = 0;
                float focalDist = 1;

                while (fgets(line, s_MAX_LINE_LENGTH, file))
                {
                    // end group
                    if (strchr(line, '}')) {
                        break;
                    }

                    scanf_s (line, " position %f %f %f", &position.x, &position.y, &position.z);
                    scanf_s (line, " lookAt %f %f %f", &lookAt.x, &lookAt.y, &lookAt.z);
                    scanf_s (line, " aperture %f ", &aperture);
                    scanf_s (line, " focaldist %f", &focalDist);
                    scanf_s (line, " fov %f", &fov);
                }

                scene->AddCamera(position, lookAt, fov);
                scene->camera->aperture = aperture;
                scene->camera->focalDist = focalDist;

                cameraAdded = true;
            }

            //--------------------------------------------
            // Renderer
            if (strstr(line, "Renderer"))
            {
                char envMap[200] = "None";

                while (fgets(line, s_MAX_LINE_LENGTH, file))
                {
                    // end group
                    if (strchr(line, '}')) {
                        break;
                    }

                    scanf_s (line, " envMap %s", envMap);
                    scanf_s (line, " resolution %f %f", &renderOptions.WindowSize.x, &renderOptions.WindowSize.y);
                    scanf_s (line, " hdrMultiplier %f", &renderOptions.intensity);
                    scanf_s (line, " maxDepth %i", &renderOptions.maxDepth);
                    scanf_s (line, " numTilesX %i", &renderOptions.numTilesX);
                    scanf_s (line, " numTilesY %i", &renderOptions.numTilesY);
                }

                if (strcmp(envMap, "None") != 0)
                {
                    scene->AddHDR(rootPath + envMap);
                    renderOptions.useEnvMap = true;
                }
            }

            //--------------------------------------------
            // Mesh
            if (strstr(line, "mesh"))
            {
                std::string filename;
                Vector3 pos;
                Vector3 scale;
                Matrix4x4 xform;
                int materialID = 0; // Default Material ID

                while (fgets(line, s_MAX_LINE_LENGTH, file))
                {
                    // end group
                    if (strchr(line, '}')) {
                        break;
                    }

                    char file[2048];
                    char matName[100];

                    if (scanf_s (line, " file %s", file) == 1) {
                        filename = file;
                    }

                    if (scanf_s (line, " material %s", matName) == 1)
                    {
                        // look up material in dictionary
                        if (materialMap.find(matName) != materialMap.end())
                        {
                            materialID = float(materialMap[matName].id);
                        }
                        else
                        {
                            printf("Could not find material %s\n", matName);
                        }
                    }

                    scanf_s (line, " position %f %f %f", &xform.m[3][0], &xform.m[3][1], &xform.m[3][2]);
                    scanf_s (line, " scale %f %f %f", &xform.m[0][0], &xform.m[1][1], &xform.m[2][2]);
                }

                if (!filename.empty())
                {
                    int meshID = scene->AddMesh(rootPath + filename);
                    if (meshID != -1)
                    {
                        std::string baseName = filename.substr(filename.find_last_of("/\\") + 1);
                        scene->AddMeshInstance(MeshInstance(meshID, xform, materialID, baseName));
                    }
                }
            }
        }

        fclose(file);

        // Add default camera if none was specified
        if (!cameraAdded) {
            scene->AddCamera(Vector3(0.0f, 0.0f, 10.0f), Vector3(0.0f, 0.0f, -10.0f), 35.0f);
        }

        renderOptions.frameSize = renderOptions.WindowSize;

        scene->CreateAccelerationStructures();

        return true;
    }

}