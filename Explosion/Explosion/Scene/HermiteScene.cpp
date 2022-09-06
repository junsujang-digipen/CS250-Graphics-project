/*
Junsu Jang, Seongwon Jang
CS250
Class Project
2022 Spring
*/
#include "HermiteScene.h"

#include "../GraphicEngine/Function.h"
#include "../GraphicEngine/DefaultMesh.h"
#include "../GraphicEngine/Noise.h"
#include "../GraphicEngine/GraphicEngine.h"
#include "Imgui/imgui.h"

glm::mat3x3 vpToNdc()
{
	return glm::inverse(glm::mat3{
		glm::vec3{GraphicEngine::windowSize.x / 2.f,0,0},
		glm::vec3{0,GraphicEngine::windowSize.y / 2.f,0},
		glm::vec3{ GraphicEngine::windowSize.x / 2.f ,
				GraphicEngine::windowSize.y / 2.f ,1.f} });
}
glm::vec3 getMouseVector() {
	glm::vec3 viewFinderVec{ glm::mat3{
		{GraphicEngine::getCamera().getCameraWidth() / 2.f,0,0},
		{0,GraphicEngine::getCamera().getCameraHeight() / 2.f,0},
		{0,
		0,
		1}}
	*glm::vec3{vpToNdc() * glm::vec3{ImGui::GetMousePos().x,GraphicEngine::windowSize.y - ImGui::GetMousePos().y,1}} };
	
	glm::vec4 worldMouseVec{ 
		GraphicEngine::getCamera().getCameraToWorld() 
		* glm::vec4{viewFinderVec.x,viewFinderVec.y,-GraphicEngine::getCamera().getNear(),1} };
	return glm::vec3{ worldMouseVec }
	- GraphicEngine::getCamera().getCameraPos() ;
}

int HermiteScene::PointInLine = { 10 };
unsigned int HermiteScene::Lnum = { 0 };
void HermiteScene::GenerateHermitePoint(glm::vec3 pos,std::string name)
{
	HermitePoint* temp{ new HermitePoint{pos} };
	temp->selfObj = new Entity(name);
	temp->selfObj->SetShader(EP::Shader::GetShader("3DDefaultShader"));
	temp->selfObj->SetMesh(EP::Mesh::GetMesh(EP::Mesh::GetSphere(64, 64)));
	temp->selfObj->SetPos(pos);
	temp->selfObj->SetColor(glm::vec4{0.4f,0.4f,1.f,1.f});
	//backPlane->SetRotate({ PI * 0.5f,0,0 });
	temp->selfObj->SetScale({ HermitePoint::selfSize,HermitePoint::selfSize,HermitePoint::selfSize });
	hermitePoints.push_back(temp);
	AddModel(temp->selfObj);
}

void HermiteScene::GenerateHermiteLine(HermitePoint* begin, HermitePoint* end)
{
	begin->begin = glm::vec3{ 0,100.f,0.f };
	begin->BVectorObj = new Entity{ "Circle" };
	begin->BVectorObj->SetShader(EP::Shader::GetShader("3DDefaultShader"));
	begin->BVectorObj->SetMesh(EP::Mesh::GetMesh(EP::Mesh::GetSphere(64, 64)));
	begin->BVectorObj->SetPos(begin->pos + begin->begin);
	begin->BVectorObj->SetColor(glm::vec4{ 0.8f,0.f,0.f,1.f });
	begin->BVectorObj->SetScale({ HermitePoint::VecSize,HermitePoint::VecSize,HermitePoint::VecSize });
	//begin->generateBVecLine();
	begin->BLineMeshName = GetLine(begin->pos, begin->pos + begin->begin, std::to_string(Lnum++));
	{
		Entity* tempLineB{};
		tempLineB = new Entity{ "HELLO" };
		tempLineB->SetShader(EP::Shader::GetShader("3DDefaultShader"));
		tempLineB->SetMesh(EP::Mesh::GetMesh(begin->BLineMeshName));
		tempLineB->SetColor(glm::vec4{ 1.f,1.f,1.f,1.f });
		tempLineB->SetScale({ 1,1,1 });
		AddModel(tempLineB);
	}
	AddModel(begin->BVectorObj);

	end->end = glm::vec3{ 0,100.f,0.f };
	end->EVectorObj = new Entity{ "Circle" };
	end->EVectorObj->SetShader(EP::Shader::GetShader("3DDefaultShader"));
	end->EVectorObj->SetMesh(EP::Mesh::GetMesh(EP::Mesh::GetSphere(64, 64)));
	end->EVectorObj->SetPos(end->pos - end->end);
	end->EVectorObj->SetColor(glm::vec4{ 0.f,0.8f,0.f,1.f });
	end->EVectorObj->SetScale({ HermitePoint::VecSize,HermitePoint::VecSize,HermitePoint::VecSize });
	//end->generateEVecLine();
	end->ELineMeshName = GetLine(end->pos, end->pos - end->end, std::to_string(Lnum++));
	{
		Entity* tempLineE{};
		tempLineE = new Entity{ "" };
		tempLineE->SetShader(EP::Shader::GetShader("3DDefaultShader"));
		tempLineE->SetMesh(EP::Mesh::GetMesh(end->ELineMeshName));
		tempLineE->SetColor(glm::vec4{ 1.f,1.f,1.f,1.f });
		tempLineE->SetScale({ 1,1,1 });
		AddModel(tempLineE);
	}
	AddModel(end->EVectorObj);

	HermiteLine* tempL{ new HermiteLine{begin,end,""} };
	{
		tempL->LineMeshName = GetHermitLine(PointInLine, *begin, *end);
		Entity* temp{};
		temp = new Entity{ "EVec" };
		temp->SetShader(EP::Shader::GetShader("3DDefaultShader"));
		temp->SetMesh(EP::Mesh::GetMesh(tempL->LineMeshName));
		//temp->SetPos((begin->pos + end->pos) / 2.f);
		temp->SetColor(glm::vec4{ 1.f,1.f,1.f,1.f });
		temp->SetScale({ 1,1,1 });
		AddModel(temp);
	}
	//begin->BLineMeshName = tempL.LineMeshName;
	//end->ELineMeshName = tempL.LineMeshName;
	hermiteLines.push_back(tempL);
}

void HermiteScene::setup()
{
	EP::Shader::MakeShaderHandle("3DDefaultShader");
	EP::Shader::CompileShader("3DDefaultShader", GL_VERTEX_SHADER, "../shaders/3Dmodel.vert");
	EP::Shader::CompileShader("3DDefaultShader", GL_FRAGMENT_SHADER, "../shaders/3Dmodel.frag");
	EP::Shader::LinkingShader("3DDefaultShader");

	//backPlane = new Entity("backGround");
	//backPlane->SetShader(EP::Shader::GetShader("3DDefaultShader"));
	//backPlane->SetMesh(EP::Mesh::GetMesh(EP::Mesh::GetPlane(128, 128)));
	//backPlane->SetPos({ 0, 0,-1000.f });
	//backPlane->SetColor({ 1.f,0.7f,0.7f,1.0f });
	//backPlane->SetScale({ 20000,20000,1 });
	//AddModel(backPlane);
	
	GenerateHermitePoint(glm::vec3{-300,0,0},"Circle");
	GenerateHermitePoint(glm::vec3{ 300,0,0 }, "Circle");
	GenerateHermiteLine(hermitePoints[0], hermitePoints[1]);


	
	GraphicEngine::getCamera().setPosition({ 0.f, 0.f, 1000.f });
	GraphicEngine::getCamera().resetAngle();
	//GraphicEngine::getCamera().setisMove(false);
	glLineWidth(lineWidth);

	glEnable(GL_DEPTH_TEST); // 
	glDepthMask(GL_TRUE); // depth
	glDepthFunc(GL_LESS); //

	
}
void HermiteScene::update(double dt)
{
	//std::cout << "x: " << ImGui::GetMousePos().x << std::endl;
	//std::cout << "y: " << ImGui::GetMousePos().y << std::endl;

	glm::vec3 mouseVec{getMouseVector()};
	std::pair<Entity*, float> candidate{nullptr,0.f};
	for (auto& e: hermitePoints) {
		if (e->selfObj !=  nullptr) {
			float t = computeRT(GraphicEngine::getCamera().getCameraPos(), mouseVec, e->selfObj);
			if (t >= 0) {
				if (candidate.first == nullptr) {
					candidate.first = e->selfObj;
					candidate.second = t;
				}
				else if(t<candidate.second) {
					candidate.first = e->selfObj;
					candidate.second = t;
				}
				e->isChanged = true;
			}
		}
		/*if (e->BVectorObj != nullptr) {
			float t = computeRT(GraphicEngine::getCamera().getCameraPos(), mouseVec, e->BVectorObj);
			if (t >= 0) {
				if (candidate.first == nullptr) {
					candidate.first = e->BVectorObj;
					candidate.second = t;
				}
				else if (t < candidate.second) {
					candidate.first = e->BVectorObj;
					candidate.second = t;
				}
				e->isChanged = true;
			}
		}
		if (e->EVectorObj != nullptr) {
			float t = computeRT(GraphicEngine::getCamera().getCameraPos(), mouseVec, e->EVectorObj);
			if (t >= 0) {
				if (candidate.first == nullptr) {
					candidate.first = e->EVectorObj;
					candidate.second = t;
				}
				else if (t < candidate.second) {
					candidate.first = e->EVectorObj;
					candidate.second = t;
				}
				e->isChanged = true;
			}
		}*/
	}
	if (candidate.first != nullptr 
		&& (ImGui::GetMouseDragDelta(ImGuiMouseButton_::ImGuiMouseButton_Right).x != 0.f
		|| ImGui::GetMouseDragDelta(ImGuiMouseButton_::ImGuiMouseButton_Right).y != 0.f)) {
		std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!11111" << std::endl;
		candidate.first->SetPos(GraphicEngine::getCamera().getCameraPos() + candidate.second*glm::normalize(mouseVec));
		
	}

	//hermitePoints[0].ChangePos(hermitePoints[0].selfObj->GetPos() + glm::vec3{ 100 * dt,20 * dt,0 });
	//hermitePoints[0].ChangeBVec(hermitePoints[0].begin + glm::vec3{ -10 * dt,-10 * dt,0 });
	//hermitePoints[1].ChangeEVec(hermitePoints[1].end - glm::vec3{ -10 * dt,-10 * dt,0 });
	for (auto a : hermiteLines)
	{
		a->updateLineMesh();
	}
		for (auto a : hermiteLines)
		{
			a->updateLineMesh2();
		}
	//std::cout << hermitePoints[0].selfObj->GetPos().x << std::endl;
	Scene::update(dt);

}

void HermiteScene::draw()
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	Scene::draw();
}


void HermiteScene::unload()
{
	Scene::unload();
	GraphicEngine::getCamera().setisMove(true);
	for (auto&p: hermitePoints) {
		delete p;
		p = nullptr;
	}
	for (auto& l : hermiteLines) {
		delete l;
		l = nullptr;
	}
	hermitePoints.clear();
	hermiteLines.clear();
}

float HermiteScene::computeRT(glm::vec3 stPoint, glm::vec3 Rvector, Entity* obj)
{
	//std::cout <<"x: "<< Rvector.x 
		//<< "y: " << Rvector.y 
		//<< "z: " << Rvector.z << std::endl;
	glm::vec3 stToObj{obj->GetPos() - stPoint};
	float p{glm::dot(stToObj,Rvector)};
	float m{p/glm::length(Rvector)  };
	float n{ (glm::dot(stToObj,stToObj) - m*m)};
	std::cout << " n: "<<n<<" m: "<<m << std::endl;
	//std::cout << obj->GetScale().x << std::endl;
	if (obj->GetScale().x/2.f* obj->GetScale().x / 2.f >= n) {
		return m;
	}
	return -1.f;
}


void HermiteScene::ImguiUpdate(double /*dt*/)
{
	if (ImGui::BeginMainMenuBar())
	{

		if (ImGui::BeginMenu("Hermite"))
		{
			GraphicEngine::getCamera().setisMove(false);
			int pil = HermiteScene::PointInLine;
			ImGui::SliderInt("PointInLine",&pil,1,100);
			ImGui::SliderFloat("lineWidth", &lineWidth, 1, 15);
			glLineWidth(lineWidth);
			for (int i = 0; i < hermiteLines.size(); ++i)
			{
				if(pil != HermiteScene::PointInLine)
				{
					HermiteScene::PointInLine = pil;
					hermiteLines[i]->begin->isChanged = true;
				}
					glm::vec3 beginpos = hermiteLines[i]->begin->pos;
					glm::vec3 endpos = hermiteLines[i]->end->pos;
					glm::vec3 beginvec = hermiteLines[i]->begin->begin;
					glm::vec3 endvec = hermiteLines[i]->end->end;
					std::string num{ "Line " + std::to_string(i)};
					ImGui::Text({ num.c_str() });
					ImGui::SliderFloat3(std::string{ "BeginPos" +std::to_string(i)}.c_str(), &beginpos.x, -200.f, 200.f);
					ImGui::SliderFloat3(std::string{ "EndPos" + std::to_string(i) }.c_str(), &endpos.x, -200.f, 200.f);
					ImGui::SliderFloat3(std::string{ "BeginVec" + std::to_string(i) }.c_str(), &beginvec.x, -200.f, 200.f);
					ImGui::SliderFloat3(std::string{ "EndVec" + std::to_string(i) }.c_str(), &endvec.x, -200.f, 200.f);

					hermiteLines[i]->begin->ChangePos(beginpos);
					hermiteLines[i]->begin->ChangeBVec(beginvec);
					hermiteLines[i]->end->ChangePos(endpos);
					hermiteLines[i]->end->ChangeEVec(endvec);

			}
			ImGui::EndMenu();
		}
		else {
			GraphicEngine::getCamera().setisMove(true);
		}
		if (ImGui::BeginMenu("Add point")) {
			static glm::vec3 pos{};
			ImGui::InputFloat3("Position", &pos.x);
				
			if (ImGui::Button("Add")) {
				GenerateHermitePoint(pos, "point");
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Make line")) {
			static HermitePoint* beginPtr{ nullptr };
			static HermitePoint* endPtr{ nullptr };

			ImGui::Text({ "Begin point" });
			for (int i = 0;i < hermitePoints.size();++i) {
				if (hermitePoints[i]->BVectorObj == nullptr&& endPtr != hermitePoints[i]) {
					std::string num{ "Begin Point " + std::to_string(i) };
					if (ImGui::Button(num.c_str())) {
						if (beginPtr != nullptr) {
							beginPtr->selfObj->SetColor(glm::vec4{ 0.4f,0.4f,1.f,1.f });
							beginPtr->selfObj->SetScale(glm::vec3{ HermitePoint::selfSize  });
						}
						beginPtr = hermitePoints[i];
						beginPtr->selfObj->SetColor(glm::vec4{ 1.f,1.f,1.f,1.f });
						beginPtr->selfObj->SetScale(glm::vec3{ HermitePoint::selfSize* 1.5f});
					}
				}
			}
			ImGui::Text("End point");
			for (int i = 0;i < hermitePoints.size();++i) {
				if (hermitePoints[i]->EVectorObj == nullptr && beginPtr!=hermitePoints[i]) {
					std::string num{ "End Point " + std::to_string(i) };
					if (ImGui::Button(num.c_str())) {
						if (endPtr != nullptr) {
							endPtr->selfObj->SetColor(glm::vec4{ 0.4f,0.4f,1.f,1.f });
							endPtr->selfObj->SetScale(glm::vec3{ HermitePoint::selfSize  });
						}
						endPtr = hermitePoints[i];
						endPtr->selfObj->SetColor(glm::vec4{ 1.f,1.f,1.f,1.f });
						endPtr->selfObj->SetScale(glm::vec3{ HermitePoint::selfSize * 1.5f});
					}
				}
			}
			if (ImGui::Button("Make") && beginPtr != nullptr && endPtr != nullptr && beginPtr != endPtr) {
				GenerateHermiteLine(beginPtr,endPtr);
				beginPtr->selfObj->SetColor(glm::vec4{ 0.4f,0.4f,1.f,1.f });
				beginPtr->selfObj->SetScale(glm::vec3{ HermitePoint::selfSize });
				endPtr->selfObj->SetColor(glm::vec4{ 0.4f,0.4f,1.f,1.f });
				endPtr->selfObj->SetScale(glm::vec3{ HermitePoint::selfSize });
				beginPtr = nullptr;
				endPtr = nullptr;
			}
			ImGui::EndMenu();
		}
		/*else {
			for (int i = 0;i < hermitePoints.size();++i) {
				hermitePoints[i].selfObj->SetColor(glm::vec4{});
			}
		}*/
		ImGui::EndMainMenuBar();
	}

}

void HermitePoint::ChangePos(glm::vec3 p)
{
	pos = p;

	selfObj->SetPos(pos);
	if (BVectorObj != nullptr) {	
	   BVectorObj->SetPos(pos + begin);
	}
	if (EVectorObj != nullptr) {
		EVectorObj->SetPos(pos - end);

	}
	isChanged = true;
}

void HermitePoint::ChangeBVec(glm::vec3 v)
{
	if(BVectorObj == nullptr)
	{
		std::cout << "bvec doesn't exist\n";
		return;
	}
	BVectorObj->SetPos(pos + v);
	begin = v;
	isChanged = true;
}

void HermitePoint::ChangeEVec(glm::vec3 v)
{
	if (EVectorObj == nullptr)
	{
		std::cout << "bvec doesn't exist\n";
		return;
	}
	EVectorObj->SetPos(pos - v);
	end = v;
	isChanged = true;
}



void HermitePoint::checkMouseOnSelf()
{
}

bool HermitePoint::checkMouseOnBeginVec()
{
	return false;
}

bool HermitePoint::checkMouseOnEndVec()
{
	return false;
}

void HermitePoint::updateBVecLine()
{
}

void HermitePoint::updateEVecLine()
{
}

void HermiteLine::updateLineMesh()
{
	if (begin != nullptr)
	{
		if (begin->isChanged == true)
		{

			ChangeMesh();


			ChangeLine(begin->pos, begin->pos + begin->begin, begin->BLineMeshName);
			ChangeLine(end->pos, end->pos - end->end, end->ELineMeshName);
			begin->isChanged = false;
		}
	}
}

void HermiteLine::updateLineMesh2()
{
	if (end != nullptr)
	{
		if ( end->isChanged == true)
		{
			ChangeMesh();
			ChangeLine(begin->pos, begin->pos + begin->begin, begin->BLineMeshName);
			ChangeLine(end->pos, end->pos - end->end, end->ELineMeshName);
			end->isChanged = false;
		}
	}
}

void HermiteLine::ChangeMesh()
{
	ChangeHermiteCurve(LineMeshName,HermiteScene::PointInLine, begin, end);
}
