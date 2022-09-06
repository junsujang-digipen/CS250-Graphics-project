/*
Junsu Jang, Seongwon Jang
CS250
Class Project
2022 Spring
*/

#include "NoiseScene.h"
#include "../GraphicEngine/Function.h"
#include "../GraphicEngine/DefaultMesh.h"
#include "../GraphicEngine/Noise.h"
#include "../GraphicEngine/GraphicEngine.h"
#include "Imgui/imgui.h"
#include "../GraphicEngine/Mesh.h"


void NoiseScene::setup()
{
	time = GraphicEngine::GetTime();
	
	EP::Shader::MakeShaderHandle("3DDefaultShader");
	EP::Shader::CompileShader("3DDefaultShader", GL_VERTEX_SHADER, "../shaders/3Dmodel.vert");
	EP::Shader::CompileShader("3DDefaultShader", GL_FRAGMENT_SHADER, "../shaders/3Dmodel.frag");
	EP::Shader::LinkingShader("3DDefaultShader");
	EP::Shader::MakeShaderHandle("NoiseShader");
	EP::Shader::CompileShader("NoiseShader", GL_VERTEX_SHADER, "../shaders/NoiseShdr.vert");
	EP::Shader::CompileShader("NoiseShader", GL_FRAGMENT_SHADER, "../shaders/NoiseShdr.frag");
	EP::Shader::LinkingShader("NoiseShader");

	EP::Shader::MakeShaderHandle("texShader");
	EP::Shader::CompileShader("texShader", GL_VERTEX_SHADER, "../shaders/textureShdr.vert");
	EP::Shader::CompileShader("texShader", GL_FRAGMENT_SHADER, "../shaders/textureShdr.frag");
	EP::Shader::LinkingShader("texShader");

	texShdr = &(EP::Shader::GetShader("texShader")->glslshader);

	object = new Entity("Noise_plane");
	object->SetShader(EP::Shader::GetShader("texShader"));
	object->SetMesh(EP::Mesh::GetMesh(EP::Mesh::GetPlane(256,256)));
	object->SetPos({ 1000.f, 1000.f,300.f });
	object->SetColor({ 0.f,0.f,0.f,1.0f });
	object->SetRotate({ 0,0,0 });
	object->SetScale({ 200,200,50 });

	backPlane = new Entity("backGround");
	backPlane->SetShader(EP::Shader::GetShader("3DDefaultShader"));
	backPlane->SetMesh(EP::Mesh::GetMesh(EP::Mesh::GetPlane(128, 128)));
	backPlane->SetPos({ 1000.f, 1000.f,100.f });
	backPlane->SetColor({ 1.f,0.7f,0.7f,1.0f });
	//backPlane->SetRotate({ PI * 0.5f,0,0 });
	backPlane->SetScale({ 2000,2000,1 });

	
	xyzAxis.push_back(new Entity("x_axis"));
	xyzAxis[0]->SetShader(EP::Shader::GetShader("3DDefaultShader"));
	xyzAxis[0]->SetMesh(EP::Mesh::GetMesh(EP::Mesh::GetPlane(100,100)));
	xyzAxis[0]->SetPos({ 0, 0,0 });
	xyzAxis[0]->SetColor({ 1.f,0.f,0.f,0.1f });
	//xyzAxis[0]->SetRotate({ 0,PI * 0.5f,0 });
	xyzAxis[0]->SetScale({ 10000,10000,10000 });

	xyzAxis.push_back(new Entity("y_axis"));
	xyzAxis[1]->SetShader(EP::Shader::GetShader("3DDefaultShader"));
	xyzAxis[1]->SetMesh(EP::Mesh::GetMesh(EP::Mesh::GetPlane(100, 100)));
	xyzAxis[1]->SetPos({ 0, 0,0 });
	xyzAxis[1]->SetColor({ 0.f,0.f,1.f,0.1f });
	//xyzAxis[1]->SetRotate({ 0,0,PI*0.5f });
	xyzAxis[1]->SetScale({ 10000,10000,10000 });

	xyzAxis.push_back(new Entity("z_axis"));
	xyzAxis[2]->SetShader(EP::Shader::GetShader("3DDefaultShader"));
	xyzAxis[2]->SetMesh(EP::Mesh::GetMesh(EP::Mesh::GetPlane(100, 100)));
	xyzAxis[2]->SetPos({ 0, 0,0 });
	xyzAxis[2]->SetColor({ 0.f,1.f,0.f,0.1f });
	//xyzAxis[2]->SetRotate({ PI*0.5f,0,0 });
	xyzAxis[2]->SetScale({ 10000,10000,10000 });

	xyzAxis[0]->SetDrawMode(GL_LINE);
	xyzAxis[1]->SetDrawMode(GL_LINE);
	xyzAxis[2]->SetDrawMode(GL_LINE);
	AddModel(object);
	AddModel(backPlane);
	AddModel(xyzAxis[0]);
	AddModel(xyzAxis[1]);
	AddModel(xyzAxis[2]);


	GraphicEngine::getCamera().setPosition({ 1000.f, 1000.f, 600.f });
	GraphicEngine::getCamera().resetAngle();
	GraphicEngine::getCamera().setisMove(false);

	pboGene();
}

void NoiseScene::update(double dt)
{
	Scene::update(dt);
}

void NoiseScene::draw()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST); // 
	glDepthMask(GL_TRUE); // depth
	glDepthFunc(GL_LESS); //
	//glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


	
	glBindTexture(GL_TEXTURE_2D, textureId);

	//texShdr->Use();
	//object->DrawBinding();
	////if (texShdr != nullptr) 
	//{
	//	//texShdr->SetUniform("tex", static_cast<GLint>(textureId));
	//}
	//object->DrawUnBinding();
	//texShdr->UnUse();
	Scene::draw();
	glBindTexture(GL_TEXTURE_2D, 0);
}

void NoiseScene::unload()
{
	Scene::unload();
	xyzAxis.clear();
	GraphicEngine::getCamera().setisMove(true);

	pboDele();
}

void NoiseScene::ImguiUpdate(double)
{

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("NoiseShader"))
		{
			//static bool pmode = false;
			if (ImGui::BeginTabBar("Method")) {
				if (ImGui::TabItemButton("Noise Shader")) {
					noiseMth = Method::Shader;
					object->SetShader(EP::Shader::GetShader("NoiseShader"));
					object->ClearUniform();
					object->AddUniform(new tUniform("seed", seed));
					object->AddUniform(new tUniform("size", size));
					object->AddUniform(new tUniform("freq", freq));
					object->AddUniform(new tUniform("type", type));
					object->AddUniform(new tUniform("numLayer", numLayer));
					object->AddUniform(new u_modelToNDC(object->GetMatrix()));
					object->AddUniform(new u_color(object->GetColor()));
				}
				if (ImGui::TabItemButton("Value Noise")) {
					noiseMth = Method::Value;
					object->SetShader(EP::Shader::GetShader("texShader"));
					object->ClearUniform();
					object->AddUniform(new u_modelToNDC(object->GetMatrix()));
					object->AddUniform(new u_color(object->GetColor()));
					object->AddUniform(new tUniform("PlaneView", planeviewTemp));
				}
				if (ImGui::TabItemButton("Perlin Noise")) {
					noiseMth = Method::Perlin;
					object->SetShader(EP::Shader::GetShader("texShader"));
					object->ClearUniform();
					object->AddUniform(new u_modelToNDC(object->GetMatrix()));
					object->AddUniform(new u_color(object->GetColor()));
					object->AddUniform(new tUniform("PlaneView", planeviewTemp));
				}
				ImGui::EndTabBar();
			}
			
			if (noiseMth == Method::Shader) {
				ImGui::SliderFloat("seed", &seed, 10.f, 100.f);
				ImGui::SliderFloat("size", &size, 128.f, 1024.f);
			}

			ImGui::SliderInt("type", &type, 0, 5);			
			ImGui::SliderFloat("freq", &freq, 0.01f, 1.f);
			
			ImGui::SliderInt("layer", &numLayer,1, 6);

			if (noiseMth == Method::Perlin || noiseMth == Method::Value) {
				ImGui::Checkbox("Animation", &isAnimation);
				noiseDataBind();
				
			}
			if (noiseMth == Method::Perlin) {
				ImGui::InputFloat("x speed", &xSpeed);
				ImGui::InputFloat("y speed", &ySpeed);
				ImGui::InputFloat("z speed", &zSpeed);
			}
			ImGui::Checkbox("Plane view", &isPlaneView);
			planeviewTemp = static_cast<int>(isPlaneView);
			if (isPlaneView) {
				object->SetRotate({ PI * -0.2f,0,PI * -0.3f });
			}
			else {
				object->SetRotate({ 0,0,0 });
			}
			ImGui::EndMenu();
		}
		else {
			if (isAnimation == true) {
				noiseDataBind();
			}
		}


		ImGui::EndMainMenuBar();
	}


}


void NoiseScene::noiseDataBind()
{
	{
		// bind the texture and PBO
		//glBindTexture(GL_TEXTURE_2D, textureId);
		//glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pboIds);

		// copy pixels from PBO to texture object
		// Use offset instead of ponter.
		//glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, noiseSize, noiseSize, GL_BGRA, GL_UNSIGNED_BYTE, 0);


		// bind PBO to update texture source
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pboIds);

		// Note that glMapBuffer() causes sync issue.
		// If GPU is working with this buffer, glMapBuffer() will wait(stall)
		// until GPU to finish its job. To avoid waiting (idle), you can call
		// first glBufferData() with NULL pointer before glMapBuffer().
		// If you do that, the previous data in PBO will be discarded and
		// glMapBuffer() returns a new allocated pointer immediately
		// even if GPU is still working with the previous data.
		glBufferData(GL_PIXEL_UNPACK_BUFFER, noiseArrSize*sizeof(GLuint), 0, GL_STREAM_DRAW);

		// map the buffer object into client's memory
		GLubyte* ptr = (GLubyte*)glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);
		if (ptr)
		{
			// update data directly on the mapped buffer
			//updatePixels(ptr, noiseArrSize);
			if (isAnimation == true) {
				time = GraphicEngine::GetTime();
			}
			for (int h = 0;h < noiseSize;++h) {
				for (int w = 0;w < noiseSize;++w) {
					float val{};
					if (noiseMth == Method::Value) {
						val = noise2D( w * freq + time,h * freq);

					}
					if (noiseMth == Method::Perlin) {
						val = perlinNoise(glm::vec3{ w * freq + time* xSpeed,h * freq + time* ySpeed,time* zSpeed }) * 0.5f + 0.5f;
					}
					float freqMfreqMult{ freq * frequencyMult };
					float tempN{ val };
					glm::vec2 tempPos{ w,h };
					switch (type)
					{
					case 0:
						//for (int i = 0; i < numLayer; ++i) {
						//	tempN = noise2D(w * freqMfreqMult, h * freqMfreqMult) * amplitude;
						//	amplitude = amplitude * amplitudeMult;
						//	val += abs(tempN * 2 - 1);
						//}
						//val /= float(numLayer);
						break;
					case 1:
						val *= 10;
						val -= static_cast<int>(val);
						break;
					case 2:
						for (int i = 1;i < numLayer;++i) {
							//float layerSeed = (seed + 0.01f) * i;
							if (noiseMth == Method::Value) {
								//tempN = noise2D(w * freq + time, h * freq);
							}
							else if (noiseMth == Method::Perlin) {
								//tempN = perlinNoise(glm::vec3{ w * freq,h * freq,time  }) * 0.5f + 0.5f;
							}
							val += abs(tempN * 2 - 1);
						}
						val /= float(numLayer);
						break;
					case 3:
						for (int i = 1; i < numLayer; ++i) {
							if (noiseMth == Method::Value) {
								tempN = tempN + noise2D(w * freqMfreqMult + time, h * freqMfreqMult) * amplitude;
							}
							else if (noiseMth == Method::Perlin) {
								tempN = tempN + (perlinNoise(glm::vec3(w * freqMfreqMult+time * xSpeed, h * freqMfreqMult+ time * ySpeed, time * zSpeed)) * 0.5f + 0.5f) * amplitude;
							}
							amplitude = amplitude * amplitudeMult;
						}
						val = (sin((w + tempN * 100.f) * 2.f * 3.14f / 200.f) + 1.f) / 2.f;
						break;
					case 4:
						amplitude = 0.8f;
						for (int i = 1; i < numLayer; ++i) {
							
							if (noiseMth == Method::Value) {
								tempN =  noise2D(
									tempPos.x * freqMfreqMult + time, 
									tempPos.y * freqMfreqMult)* amplitude;
							}
							else if (noiseMth == Method::Perlin) {
								tempN =  (perlinNoise(glm::vec3(
									tempPos.x * freqMfreqMult + time * xSpeed,
									tempPos.y * freqMfreqMult + time * ySpeed,
									time * zSpeed)) * 0.5f + 0.5f)* amplitude;
							}
							tempPos *= 2.f;
							freqMfreqMult *= frequencyMult;
							amplitude *= amplitudeMult;
							val += tempN>1.f?1.f:tempN;
						}
						break;
					case 5:
						val = val+ static_cast<float>(sin(w * freq))* static_cast<float>(cos(h * freq));
						break;
					case 6:
						val = 1.f-val;
						break;
					default:
						break;
					}
					
					
					unsigned char col = static_cast<char>(val*255);
					//GLubyte tex{};
					//tex <<
					GLubyte* temp = ptr + h * (noiseSize * 4) + (w * 4);
					*temp = static_cast<GLubyte>(col);
					*(temp + 1) = static_cast<GLubyte>(col);
					*(temp + 2) = static_cast<GLubyte>(col);
					*(temp + 3) = static_cast<GLubyte>(col);
					//noiseArray[h * (w)+(w)] = *reinterpret_cast<GLuint*>(temp);
				}
			}
			glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER); // release the mapped buffer
		}

		glBindTexture(GL_TEXTURE_2D, textureId);
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pboIds);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, noiseSize, noiseSize, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		// it is good idea to release PBOs with ID 0 after use.
		// Once bound with 0, all pixel operations are back to normal ways.
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void NoiseScene::pboGene()
{
	//imageData = new GLubyte[noiseArrSize * sizeof(GLuint)]{};
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, noiseSize, noiseSize, 0, GL_RGBA, GL_UNSIGNED_BYTE, /*(GLvoid*)imageData*/0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenBuffers(1, &pboIds);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pboIds);
	glBufferData(GL_PIXEL_UNPACK_BUFFER, noiseArrSize * sizeof(GLuint), 0, GL_STREAM_DRAW);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
	
	glEnable(GL_TEXTURE_2D);

	noiseDataBind();
}

void NoiseScene::pboDele()
{
	glDeleteTextures(1,&textureId);
	glDeleteBuffers(1,&pboIds);
	//delete[] imageData;
}
