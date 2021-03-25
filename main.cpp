#include <Windows.h>
#include "Window.h"
#include "D3D.h"
#include<DirectXMath.h>
#include <string>

#include "Camera.h"
#include "Mesh.h"
#include "Time.h"
#include "Light.h"
#include "Material.h"
#include "GameObject.h"
#include "Skybox.h"

int ThrowErrorMSGBox(int code)
{
	MessageBox(nullptr, std::to_wstring(code).c_str(), L"Error", 0);
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE DEPRECATED_hPrevInstance, LPSTR szCmdLine, int nCmdShow)
{
	INT width = 1620;
	INT height = 980;
	bool isWindowed = true;

	Window window;
	int error = window.init(hInstance, width, height, nCmdShow);
	if (error != 0) return error;

	D3D d3d;
	error = d3d.init(window.getWindowHandle(), width, height, isWindowed);
	if (error != 0) return error;


	
	// Meshes
	Mesh meshCube;
	error = meshCube.init(d3d.getDevice(), Mesh::Object::cube);
	if (error != 0) return error;

	Mesh meshSphere;
	error = meshSphere.init(d3d.getDevice(), Mesh::Object::sphere);
	if (error != 0) return error;

	Mesh meshSkybox;
	error = meshSkybox.init(d3d.getDevice(), Mesh::Object::skybox);
	if (error != 0) return error;

	
	// Camera
	Camera camera;
	error = camera.init(width, height);
	if (error != 0) return error;

	// Time
	Time time;
	error = time.init();
	if (error != 0) return error;

	// Shader parameters
	LPCWSTR normalMapPhongVertexShader = L"NormalMapPhongVertexShader";
	LPCWSTR normalMapPhongPixelShader = L"NormalMapPhongPixelShader";
	LPCWSTR textureCubePhongVertexShader = L"TextureCubePhongVertexShader";
	LPCWSTR textureCubePhongPixelShader = L"TextureCubePhongPixelShader";
	LPCWSTR cubeTexture = L"texturePurple.png";
	LPCWSTR cubeTif = L"cubetif.tif";
	LPCWSTR spehereTexture = L"sphereColour.jpg";
	LPCWSTR sphereTif = L"spheretif.tif";
	LPCWSTR space_dds = L"space.dds";

	ShaderDetails cubeGlossyNormal;
	cubeGlossyNormal.Tiling = {1, 1};
	cubeGlossyNormal.Offset = {0, 0};
	cubeGlossyNormal.VertexShaderName = normalMapPhongVertexShader;
	cubeGlossyNormal.PixelShaderName = normalMapPhongPixelShader;
	cubeGlossyNormal.SpecularPower = {32, 0};
	cubeGlossyNormal.Glossiness = {1, 0};

	ShaderDetails cubeMattNormal;
	cubeMattNormal.Tiling = {1, 1};
	cubeMattNormal.Offset = {0, 0};
	cubeMattNormal.VertexShaderName = normalMapPhongVertexShader;
	cubeMattNormal.PixelShaderName = normalMapPhongPixelShader;
	cubeMattNormal.SpecularPower = {6, 0};
	cubeMattNormal.Glossiness = {0.5f, 0};

	ShaderDetails sphereGlossy;
	sphereGlossy.Tiling = {1, 1};
	sphereGlossy.Offset = {0, 0};
	sphereGlossy.VertexShaderName = textureCubePhongVertexShader;
	sphereGlossy.PixelShaderName = textureCubePhongPixelShader;
	sphereGlossy.SpecularPower = {256.0f, 0};
	sphereGlossy.Glossiness = {0.1f, 0};

	ShaderDetails sphereMatt;
	sphereMatt.Tiling = {1, 1};
	sphereMatt.Offset = {0, 0};
	sphereMatt.VertexShaderName = textureCubePhongVertexShader;
	sphereMatt.PixelShaderName = textureCubePhongPixelShader;
	sphereMatt.SpecularPower = {0.001f, 0};
	sphereMatt.Glossiness = {0.01f, 0};

	ShaderDetails skyboxParam;
	skyboxParam.Tiling = {1, 1};
	skyboxParam.Offset = {0, 0};
	skyboxParam.VertexShaderName = textureCubePhongVertexShader;
	skyboxParam.PixelShaderName = textureCubePhongPixelShader;
	skyboxParam.SpecularPower = {0.1f, 0};
	skyboxParam.Glossiness = {0.01f, 0};

	
	// Materials
	Material cubeMattMaterial;
	error = cubeMattMaterial.init(d3d.getDevice(), cubeTexture, &cubeMattNormal, cubeTif);
	if (error != 0) return ThrowErrorMSGBox(error);

	Material cubeGlossyMaterial;
	error = cubeGlossyMaterial.init(d3d.getDevice(), cubeTexture, &cubeGlossyNormal,cubeTif);
	if (error != 0) return ThrowErrorMSGBox(error);

	Material sphereMatertial1;
	error = sphereMatertial1.init(d3d.getDevice(), spehereTexture, &sphereGlossy, sphereTif);
	if (error != 0) return ThrowErrorMSGBox(error);

	Material sphereMatertial2;
	error = sphereMatertial2.init(d3d.getDevice(), spehereTexture, &sphereMatt, sphereTif);
	if (error != 0) return ThrowErrorMSGBox(error);

	Material skyboxMaterial;
	error = skyboxMaterial.init(d3d.getDevice(), space_dds, &skyboxParam, NULL);
	if (error != 0) return ThrowErrorMSGBox(error);

	Skybox skybox;
	error = skybox.init(d3d.getDevice(), d3d.getDeviceContext(), L"space.dds");
	if (error != 0) return error;

	
	// GameObjects
	GameObject cube1GO;
	error = cube1GO.init(&cubeMattMaterial, &meshCube);
	if (error != 0) return ThrowErrorMSGBox(error);
	cube1GO.setPos(0.1f, -0.5f, 0.0f);
	cube1GO.setScale(0.5f, 0.5f, 0.5f);

	GameObject cube2GO;
	error = cube2GO.init(&cubeGlossyMaterial, &meshCube);
	if (error != 0) return ThrowErrorMSGBox(error);
	cube2GO.setPos(-0.5f, -0.5f, 0.0f);
	cube2GO.setScale(0.5f, 0.5f, 0.5f);

	
	GameObject sphere1GO;
	error = sphere1GO.init(&sphereMatertial1, &meshSphere);
	if (error != 0) return ThrowErrorMSGBox(error);
	sphere1GO.setPos(-0.75f, 0.75f, 0.0f);
	sphere1GO.setScale(0.5f, 0.5f, 0.5f);

	GameObject sphere2GO;
	error = sphere2GO.init(&sphereMatertial2, &meshSphere);
	if (error != 0) return ThrowErrorMSGBox(error);
	sphere2GO.setPos(0.5f, 0.75f, 0.0f);
	sphere2GO.setScale(0.5f, 0.5f, 0.5f);

	
	GameObject skyboxGO;
	error = skyboxGO.init(&skyboxMaterial, &meshSkybox);
	if (error != 0) return ThrowErrorMSGBox(error);
	skyboxGO.setPos(0.0f, 0.0f, 0.0f);
	skyboxGO.setScale(20.0f, 20.0f, 20.0f);

	
	// Light
	Light light = {};
	light.LightDirection = {0.0f, 1.0f, 0.0f};
	light.AmbientColor = {0.1f, 0.1f, 0.1f, 1.0f};
	light.DiffuseColor = {1.0f, 1.0f, 1.0f, 1.0f};
	light.LightIntensity = 0.5f;

	cubeGlossyMaterial.setLight(d3d.getDeviceContext(), light);
	cubeMattMaterial.setLight(d3d.getDeviceContext(), light);
	skyboxMaterial.setLight(d3d.getDeviceContext(), light);
	sphereMatertial1.setLight(d3d.getDeviceContext(), light);
	sphereMatertial2.setLight(d3d.getDeviceContext(), light);


	// App
	while (true)
	{
		if (!window.run()) break;

		time.update();
		d3d.beginScene();

		sphere1GO.changeRotation(0.3f * time.getDeltaTime(), 0.9f * time.getDeltaTime(),0.7f * time.getDeltaTime());
		sphere2GO.changeRotation(0.3f * time.getDeltaTime(), 0.2f * time.getDeltaTime(),0.7f * time.getDeltaTime());

		cube1GO.update();
		cube2GO.update();

		sphere1GO.update();
		sphere2GO.update();
		
		skyboxGO.update();
		
		camera.update(time.getDeltaTime());

		cube1GO.render(d3d.getDeviceContext(), &camera);
		cube2GO.render(d3d.getDeviceContext(), &camera);
		sphere1GO.render(d3d.getDeviceContext(), &camera);
		sphere2GO.render(d3d.getDeviceContext(), &camera);
		skyboxGO.render(d3d.getDeviceContext(), &camera);


		d3d.endScene();
	}


	//DeInit
	d3d.deInit();
	cubeGlossyMaterial.deInit();
	cubeMattMaterial.deInit();
	skyboxMaterial.deInit();
	sphereMatertial1.deInit();
	sphereMatertial2.deInit();

	skybox.de_init();
	meshCube.deInit();
	d3d.deInit();
	time.deInit();

	return 0;
}
