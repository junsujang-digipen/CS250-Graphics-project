/*
Junsu Jang, Seongwon Jang
CS250
Class Project
2022 Spring
*/
#include "Catmull-romScene.h"

#include "../GraphicEngine/Function.h"
#include "../GraphicEngine/DefaultMesh.h"
#include "../GraphicEngine/Noise.h"
#include "../GraphicEngine/GraphicEngine.h"
#include "Imgui/imgui.h"

int CatmullRomScene::PointInLine{50};

void CatmullRomScene::setup()
{
	EP::Shader::MakeShaderHandle("3DDefaultShader");
	EP::Shader::CompileShader("3DDefaultShader", GL_VERTEX_SHADER, "../shaders/3Dmodel.vert");
	EP::Shader::CompileShader("3DDefaultShader", GL_FRAGMENT_SHADER, "../shaders/3Dmodel.frag");
	EP::Shader::LinkingShader("3DDefaultShader");
	

	generateNewCRLine({ -200,100,100 }, { 200,200,100 });
	GenerateCRPoint(selectedLineNum, { 0,0,100 },"CRP");
	GenerateCRPoint(selectedLineNum, { 300,-300,100 },"CRP");

	GraphicEngine::getCamera().setPosition({ 0.f, 0.f, 500.f });
	GraphicEngine::getCamera().resetAngle();
	glLineWidth(lineWidth);

	glEnable(GL_DEPTH_TEST); // 
	glDepthMask(GL_TRUE); // depth
	glDepthFunc(GL_LESS); //
}


void CatmullRomScene::update(double dt)
{
	UpdateAllCRLines();
	Scene::update(dt);

}

void CatmullRomScene::draw()
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	Scene::draw();
}


void CatmullRomScene::unload()
{
	Scene::unload();
	for (auto & l:CRLine) {
		l.points.clear();
	}
	CRLine.clear();
}


void CatmullRomScene::ImguiUpdate(double /*dt*/)
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Generate Line")) {
			static glm::vec3 pos1{};
			static glm::vec3 pos2{};
			ImGui::InputFloat3("Point 1",&pos1.x);
			ImGui::InputFloat3("Point 2", &pos2.x);
			if (ImGui::Button("Generate")) {
				generateNewCRLine( pos1,pos2);
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Add point")) {
			//ImGui::ArrowButton("",ImGuiDir_::ImGuiDir_Down);
			if (ImGui::BeginTabBar("Select line Button")) {
				for (int i = 0;i < CRLine.size();++i) {
					if (ImGui::TabItemButton(std::string{"Line " + std::to_string(i)}.c_str())) {
						selectedLineNum = i;
					}
				}
				ImGui::EndTabBar();
			}
			static glm::vec3 pos{};
			ImGui::InputFloat3("Position", &pos.x);
			if (ImGui::Button("Add")) {
				GenerateCRPoint(selectedLineNum,pos, "CRP");
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Catmull-Rom Line")) {
			if (ImGui::BeginTabBar("Select line Button")) {
				for (int i = 0;i < CRLine.size();++i) {
					if (ImGui::TabItemButton(std::string{ "Line " + std::to_string(i) }.c_str())) {
						selectedLineNum = i;
					}
				}
				ImGui::EndTabBar();
			}
			for (int i = 0;i < CRLine[selectedLineNum].points.size();++i) {
				if (ImGui::InputFloat3(std::string{ "Point " + std::to_string(i) }.c_str(), &CRLine[selectedLineNum].points[i].selfObj->GetPos().x)) {
					CRLine[selectedLineNum].points[i].selfObj->SetPos(CRLine[selectedLineNum].points[i].selfObj->GetPos());
					CRLine[selectedLineNum].isChanged = true;
				}
			}

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

}

glm::vec3 CRLine::getCRLinePoint(double t, bool isLoop)
{
	int p0, p1, p2, p3;
	if (!isLoop)
	{
		p1 = (int)t + 1;
		p2 = p1 + 1;
		p3 = p2 + 1;
		p0 = p1 - 1;
	}
	else
	{
		p1 = (int)t % points.size();
		p2 = (p1 + 1) % points.size();
		p3 = (p2 + 1) % points.size();
		p0 = p1 >= 1 ? p1 - 1 : static_cast<int>(points.size()) - 1;
	}

	t = t - (int)t;

	float tt = static_cast<float>(t * t);
	float ttt = tt * static_cast<float>(t);

	float q1 = -ttt + 2.0f * tt - static_cast<float>(t);
	float q2 = 3.0f * ttt - 5.0f * tt + 2.0f;
	float q3 = -3.0f * ttt + 4.0f * tt + static_cast<float>(t);
	float q4 = ttt - tt;

	glm::vec3 tVec = 0.5f * (points[p0].selfObj->GetPos() * q1 + points[p1].selfObj->GetPos() * q2 + points[p2].selfObj->GetPos() * q3 + points[p3].selfObj->GetPos() * q4);
	return { tVec };
}

void CRLine::UpdateCRLine()
{
	ChangeCRLine(LineMeshName, CatmullRomScene::PointInLine,*this);
	isChanged = false;
}

void CatmullRomScene::GenerateCRPoint(int lineNum, glm::vec3 pos, std::string name)
{
	CRPoint temp{ CRPoint{} };
	temp.selfObj = new Entity(name);
	temp.selfObj->SetShader(EP::Shader::GetShader("3DDefaultShader"));
	temp.selfObj->SetMesh(EP::Mesh::GetMesh(EP::Mesh::GetSphere(64, 64)));
	temp.selfObj->SetPos(pos);
	temp.selfObj->SetColor(glm::vec4{ 0.4f,0.4f,1.f,1.f });
	//backPlane->SetRotate({ PI * 0.5f,0,0 });
	temp.selfObj->SetScale({ CRPoint::selfSize,CRPoint::selfSize,CRPoint::selfSize });
	
	AddModel(temp.selfObj);
	CRLine[lineNum].points.push_back(temp);
	CRLine[lineNum].isChanged = true;
}

void CatmullRomScene::generateNewCRLine(glm::vec3 pos1, glm::vec3 pos2)
{
	CRLine.emplace_back();
	selectedLineNum = static_cast<int>(CRLine.size()) - 1;
	CRLine[selectedLineNum].isChanged = true;
	GenerateCRPoint(selectedLineNum,pos1,"CRP");
	GenerateCRPoint(selectedLineNum,pos2,"CRP");

	CRLine[selectedLineNum].LineMeshName = GetCRLine(PointInLine, CRLine[selectedLineNum]);

	Entity* temp = new Entity{ {"CRL"},{} };
	temp->SetShader(EP::Shader::GetShader("3DDefaultShader"));
	temp->SetMesh(EP::Mesh::GetMesh(CRLine[selectedLineNum].LineMeshName));
	temp->SetColor({ 0.5f,1.0f,0.5f,255.0f });
	CRLine[selectedLineNum].lineObj = temp;
	AddModel(temp);

}

void CatmullRomScene::UpdateAllCRLines()
{
	for (int i = 0; i < CRLine.size();++i) {
		if (CRLine[i].isChanged == true) {
			CRLine[i].UpdateCRLine();
		}
		if (i == selectedLineNum) {
			CRLine[i].lineObj->SetColor({255,255,255,255});
		}
		else {
			CRLine[i].lineObj->SetColor({ 0,0,0,255 });
		}
	}
}
