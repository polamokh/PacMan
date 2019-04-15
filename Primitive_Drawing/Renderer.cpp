#include "Renderer.h"

#define RED 1.0f, 0.0f, 0.0f
#define GREEN 0.0f, 1.0f, 0.0f
#define BLUE 0.0f, 0.0f, 1.0f
#define GRAY 0.6f, 0.6f, 0.6f
#define BLACK 0.0f, 0.0f, 0.0f
#define WHITE 1.0f, 1.0f, 1.0f

float Dots[] = { -0.85, +0.65, -0.85, +0.45, -0.85, +0.25, -0.85, +0.05, -0.85, -0.25, -0.85, -0.45, -0.85, -0.65, -0.85, -0.85,
+0.00, +0.65, +0.00, +0.45, +0.00, +0.25, +0.00, +0.05, +0.00, -0.25, +0.00, -0.45, +0.00, -0.65, +0.00, -0.85,
+0.85, +0.65, +0.85, +0.45, +0.85, +0.25, +0.85, +0.05, +0.85, -0.25, +0.85, -0.45, +0.85, -0.65, +0.85, -0.85,

-0.65, -0.85, -0.45, -0.85, -0.25, -0.85, +0.25, -0.85, +0.45, -0.85, +0.65, -0.85,
-0.65, -0.05, -0.45, -0.05, -0.25, -0.05, +0.25, -0.05, +0.45, -0.05, +0.65, -0.05,
-0.65, +0.85, -0.45, +0.85, -0.25, +0.85, +0.25, +0.85, +0.45, +0.85, +0.65, +0.85 };

Renderer::Renderer()
{

}

Renderer::~Renderer()
{
	Cleanup();
}

void Renderer::Initialize()
{

	// Creating Vertex-Array-Object (VAO). Not used here, check the handout to see its importance.
	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);

	///////////////////////////////////////////////////////////////////////////////////////////
	/// Start Drawing your primitive
	////////////////////////////////
	FloorMat = new Texture("Floor_Mat.bmp", 0);

	BorderFront = new Texture("Border_Front.bmp", 0);
	BorderSide = new Texture("Border_Side.bmp", 0);

	PacManTexture = new Texture("PacMan_Object.bmp", 0);
	LinkyTexture = new Texture("Linky_Object.bmp", 0);
	BlinkyTexture = new Texture("Blinky_Object.bmp", 0);
	ClydeTexture = new Texture("Clyde_Object.bmp", 0);


	// Set the background color to white
	glClearColor(BLACK, 1.0f);

	static const GLfloat verts[] = {
		//%%%%%%%%%%%%%%%%%%%%%%%%%%% Q1 Object Points %%%%%%%%%%%%%%%%%%%%%%%%%
		-0.10f, +0.00f, -0.10f, WHITE, 0.0f, 0.0f,
		+0.10f, +0.00f, -0.10f, WHITE, 1.0f, 0.0f,
		+0.10f, +0.00f, +0.10f, WHITE, 1.0f, 1.0f,
		-0.10f, +0.00f, +0.10f, WHITE, 0.0f, 1.0f,
								
		-0.10f, +0.30f, -0.10f, WHITE, 0.0f, 0.0f,
		+0.10f, +0.30f, -0.10f, WHITE, 1.0f, 0.0f,
		+0.10f, +0.30f, +0.10f, WHITE, 1.0f, 1.0f,
		-0.10f, +0.30f, +0.10f, WHITE, 0.0f, 1.0f,

		//%%%%%%%%%%%%%%%%%%%%%%%%% Floor Object Points %%%%%%%%%%%%%%%%%%%%%%%%%
		-1.00f, +0.00f, -1.00f, BLACK, 0.0f, 0.0f,
		+1.00f, +0.00f, -1.00f, BLACK, 1.0f, 0.0f,
		-1.00f, +0.00f, +1.00f, BLACK, 0.0f, 1.0f,

		+1.00f, +0.00f, +1.00f, BLACK, 1.0f, 1.0f,
		+1.00f, +0.00f, -1.00f, BLACK, 1.0f, 0.0f,
		-1.00f, +0.00f, +1.00f, BLACK, 0.0f, 1.0f,

		+0.00f, +0.00f, +0.00f, BLACK, 0.0f, 0.0f,
		+0.00f, +0.00f, +0.50f, BLACK, 1.0f, 0.0f,
		+0.50f, +0.00f, +0.00f, BLACK, 0.0f, 1.0f,
	};

	//Cube details
	GLushort indices[] = { 0, 1, 2, 0, 2, 3, //bottom
		4, 5, 6, 4, 6, 7, //top
		5, 1, 2, 2, 5, 6, //right
		5, 1, 0, 5, 0, 4, //back
		4, 0, 3, 4, 3, 7, //left
		7, 3, 2, 7, 2, 6 }; //front

	glGenBuffers(1, &vertexBufferID);
	glGenBuffers(1, &myIndicesBufferID);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, myIndicesBufferID);


	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// Create and compile our GLSL program from the shaders
	programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");
	// Use our shader
	mvpMatrixID = glGetUniformLocation(programID, "MVP");
	mRenderingModeID = glGetUniformLocation(programID, "RenderingMode");

	glUseProgram(programID);

	ProjectionMatrix = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
	Cam.Reset(0, 2, 2, 
		0, 0, 0,
		0, 1, 0);
	ViewMatrix = Cam.GetViewMatrix();
	ModelMatrix = glm::mat4(1);
	MVP_matrix = ProjectionMatrix * ViewMatrix * ModelMatrix;

	//%%%%%%%%%%%%%%%%%%%%% Set Model Matrix Border And  Qup %%%%%%%%%%%%%%%%%%%%%%
	Model_Border();

	//%%%%%%%%%%%%%%%%%%%%% Set Model Matrix For PacMan Object %%%%%%%%%%%%%%%%%%%%%
	ModelPacMan = glm::translate(-0.85f, +0.0f, +0.85f) * glm::scale(+0.5f, +0.5f, +0.5f);

	//%%%%%%%%%%%%%%%%%%%%% Set Model Matrix For Linky Object %%%%%%%%%%%%%%%%%%%%%
	ModelLinky = glm::translate(0.00f, +0.0f, -0.05f) * glm::scale(+0.5f, +0.50f, +0.50f);

	//%%%%%%%%%%%%%%%%%%%%% Set Model Matrix For Clyde Object %%%%%%%%%%%%%%%%%%%%%
	ModelClyde = glm::translate(-0.85f, +0.0f, -0.85f) * glm::scale(+0.5f, +0.50f, +0.50f);

	//%%%%%%%%%%%%%%%%%%%%% Set Model Matrix For Blinky Object %%%%%%%%%%%%%%%%%%%%%
	ModelBlinky = glm::translate(+0.85f, +0.0f, -0.85f) * glm::scale(+0.5f, +0.50f, +0.50f);

	int Dots_Counter = 0;
	for (int i = 0; i < 84; i += 2)
	{
		Dots_Model[Dots_Counter] = glm::translate(Dots[i], +0.05f, Dots[i + 1]) * glm::scale(+0.20f, +0.20f, +0.20f);
		Dots_Counter++;
	}
}

void Renderer::Draw()
{
	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		sizeof(float) * 8,                  // stride
		(void*)0            // array buffer offset
	);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		sizeof(float) * 8,                  // stride
		(char*)(sizeof(float) * 3)            // array buffer offset
	);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(
		2,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		2,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		sizeof(float) * 8,                  // stride
		(char*)(sizeof(float) * 6)            // array buffer offset
	);

	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Draw OutSide Border %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	MVP_Border();

	glUniformMatrix4fv(mvpMatrixID, 1, GL_FALSE, &MVP_Out_Left[0][0]);
	Draw_Border();
	glUniformMatrix4fv(mvpMatrixID, 1, GL_FALSE, &MVP_Out_Top[0][0]);
	Draw_Border();
	glUniformMatrix4fv(mvpMatrixID, 1, GL_FALSE, &MVP_Out_Right[0][0]);
	Draw_Border();
	glUniformMatrix4fv(mvpMatrixID, 1, GL_FALSE, &MVP_Out_Down[0][0]);
	Draw_Border();
	glUniformMatrix4fv(mvpMatrixID, 1, GL_FALSE, &MVP_Q1_Left[0][0]);
	Draw_Border();
	glUniformMatrix4fv(mvpMatrixID, 1, GL_FALSE, &MVP_Q1_Right[0][0]);
	Draw_Border();
	glUniformMatrix4fv(mvpMatrixID, 1, GL_FALSE, &MVP_Q1_Down[0][0]);
	Draw_Border();
	glUniformMatrix4fv(mvpMatrixID, 1, GL_FALSE, &MVP_Q1_Top[0][0]);
	Draw_Border();
	glUniformMatrix4fv(mvpMatrixID, 1, GL_FALSE, &MVP_Q2_Left[0][0]);
	Draw_Border();
	glUniformMatrix4fv(mvpMatrixID, 1, GL_FALSE, &MVP_Q2_Right[0][0]);
	Draw_Border();
	glUniformMatrix4fv(mvpMatrixID, 1, GL_FALSE, &MVP_Q2_Down[0][0]);
	Draw_Border();
	glUniformMatrix4fv(mvpMatrixID, 1, GL_FALSE, &MVP_Q2_Top[0][0]);
	Draw_Border();
	glUniformMatrix4fv(mvpMatrixID, 1, GL_FALSE, &MVP_Q3_Left[0][0]);
	Draw_Border();
	glUniformMatrix4fv(mvpMatrixID, 1, GL_FALSE, &MVP_Q3_Right[0][0]);
	Draw_Border();
	glUniformMatrix4fv(mvpMatrixID, 1, GL_FALSE, &MVP_Q3_Down[0][0]);
	Draw_Border();
	glUniformMatrix4fv(mvpMatrixID, 1, GL_FALSE, &MVP_Q3_Top[0][0]);
	Draw_Border();
	glUniformMatrix4fv(mvpMatrixID, 1, GL_FALSE, &MVP_Q4_Left[0][0]);
	Draw_Border();
	glUniformMatrix4fv(mvpMatrixID, 1, GL_FALSE, &MVP_Q4_Right[0][0]);
	Draw_Border();
	glUniformMatrix4fv(mvpMatrixID, 1, GL_FALSE, &MVP_Q4_Down[0][0]);
	Draw_Border();
	glUniformMatrix4fv(mvpMatrixID, 1, GL_FALSE, &MVP_Q4_Top[0][0]);
	Draw_Border();

	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Draw PacMan Object %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	PacManTexture->Bind();
	MVPPacMan = Cam.GetProjectionMatrix() * Cam.GetViewMatrix() * ModelPacMan;
	glUniformMatrix4fv(mvpMatrixID, 1, GL_FALSE, &MVPPacMan[0][0]);
	mRenderingMode = 1;
	glUniform1i(mRenderingModeID, mRenderingMode);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);


	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Draw Linky Object %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	LinkyTexture->Bind();
	MVPLinky = Cam.GetProjectionMatrix() * Cam.GetViewMatrix() * ModelLinky;
	glUniformMatrix4fv(mvpMatrixID, 1, GL_FALSE, &MVPLinky[0][0]);
	mRenderingMode = 1;
	glUniform1i(mRenderingModeID, mRenderingMode);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);

	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Draw Clyde Object %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	ClydeTexture->Bind();
	MVPClyde = Cam.GetProjectionMatrix() * Cam.GetViewMatrix() * ModelClyde;
	glUniformMatrix4fv(mvpMatrixID, 1, GL_FALSE, &MVPClyde[0][0]);
	mRenderingMode = 1;
	glUniform1i(mRenderingModeID, mRenderingMode);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);

	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Draw Blinky Object %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	BlinkyTexture->Bind();
	MVPBlinky = Cam.GetProjectionMatrix() * Cam.GetViewMatrix() * ModelBlinky;
	glUniformMatrix4fv(mvpMatrixID, 1, GL_FALSE, &MVPBlinky[0][0]);
	mRenderingMode = 1;
	glUniform1i(mRenderingModeID, mRenderingMode);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);

	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Draw Floor Object %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	FloorMat->Bind();
	MVP_matrix = Cam.GetProjectionMatrix() * Cam.GetViewMatrix() * ModelMatrix;
	glUniformMatrix4fv(mvpMatrixID, 1, GL_FALSE, &MVP_matrix[0][0]);
	mRenderingMode = 0;
	glUniform1i(mRenderingModeID, mRenderingMode);
	glDrawArrays(GL_TRIANGLES, 8, 3);
	glDrawArrays(GL_TRIANGLES, 11, 3);

	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Draw Dots %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	mRenderingMode = 0;
	glUniform1i(mRenderingModeID, mRenderingMode);
	int Dots_Counter = 0;
	for (int i = 0; i < 84; i += 2)
	{
		if (Dots[i] != -1 && Dots[i + 1] != -1)
		{
			Dots_MVP[Dots_Counter] = Cam.GetProjectionMatrix() * Cam.GetViewMatrix() * Dots_Model[Dots_Counter];
			glUniformMatrix4fv(mvpMatrixID, 1, GL_FALSE, &Dots_MVP[Dots_Counter][0][0]);
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
		}

		Dots_Counter++;
	}


	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	// Binding will be written in the Draw function instead of the Initialize function if we want to draw more than one object
}

void Renderer::Update() {
	Blinky_Moveing();
	Clyde_Moveing();
	Linky_Moveing();

	//Check the collision, Go to the definition of function for details.
	CheckDynamicCollision();


}

void Renderer::HandleKeyboardInput(int K) {
	switch (K)
	{
	case GLFW_KEY_2:
		Personal_Camera = 0;
		Cam.Reset(0, 3, 0.1,
			0, 0, 0,
			0, 1, 0);
		break;
	case GLFW_KEY_3:
		Personal_Camera = 0;
		Cam.Reset(0, 2, 2,
			0, 0, 0,
			0, 1, 0);
		break;
	case GLFW_KEY_UP:
		Cam.Walk(0.1);
		break;

	case GLFW_KEY_DOWN:
		Cam.Walk(-0.1);
		break;

	case GLFW_KEY_LEFT:
		Cam.Strafe(-0.1);
		break;

	case GLFW_KEY_RIGHT:
		Cam.Strafe(0.1);
		break;
	case GLFW_KEY_W:
		Current_PacMan_Z += -0.01;
		if ((!(Current_PacMan_X > -0.85 && Current_PacMan_X < -0.02)) && (!(Current_PacMan_X > 0.02 && Current_PacMan_X < 0.85))) {
			if (Current_PacMan_Z >= -0.86) {
				ModelPacMan = glm::translate(0.0f, 0.0f, -0.01f) * ModelPacMan;
			}
			else
				Current_PacMan_Z += 0.01;
		}
		else
			Current_PacMan_Z += 0.01;

		for (int i = 0; i < 84; i += 2)
		{
			float Currant_Z = Current_PacMan_Z;
			float Currant_X = Current_PacMan_X;

			if (Currant_X >= -0.02 && Currant_X <= +0.02)
				Currant_X = 0.0;

			if ((Currant_X == Dots[i]) && Currant_Z == Dots[i + 1])
			{
				Dots[i] = -1;
				Dots[i + 1] = -1;
				PacMan_Scour++;
				break;
			}
		}
		if (Personal_Camera == 1)
			Cam.Reset(Current_PacMan_X, 0.6 + Zoom, Current_PacMan_Z + Zoom, Current_PacMan_X, 0, Current_PacMan_Z - 0.4, 0, 1, 0);
		break;

	case GLFW_KEY_S:
		Current_PacMan_Z += 0.01;
		if ((!(Current_PacMan_X > -0.85 && Current_PacMan_X < -0.02)) && (!(Current_PacMan_X > 0.02 && Current_PacMan_X < 0.85))) {
			if (Current_PacMan_Z <= 0.85) {
				ModelPacMan = glm::translate(0.0f, 0.0f, 0.01f) * ModelPacMan;
			}
			else
				Current_PacMan_Z += -0.01;
		}
		else
			Current_PacMan_Z += -0.01;

		for (int i = 0; i < 84; i += 2)
		{
			float Currant_X = Current_PacMan_X;

			if (Currant_X >= -0.02 && Currant_X <= +0.02)
				Currant_X = 0.0;

			float Currant_Z = Current_PacMan_Z;


			if ((Currant_X == Dots[i]) && Currant_Z == Dots[i + 1])
			{
				Dots[i] = -1;
				Dots[i + 1] = -1;
				PacMan_Scour++;
				break;
			}
		}
		if (Personal_Camera == 1)
			Cam.Reset(Current_PacMan_X, 0.6 + Zoom, Current_PacMan_Z + Zoom, Current_PacMan_X, 0, Current_PacMan_Z - 0.4, 0, 1, 0);
		break;

	case GLFW_KEY_D:
		Current_PacMan_X += 0.01;
		if ((!(Current_PacMan_Z > -0.85 && Current_PacMan_Z < -0.06)) && (!(Current_PacMan_Z > -0.04 && Current_PacMan_Z < 0.85))) {
			if (Current_PacMan_X <= 0.86) {
				ModelPacMan = glm::translate(0.01f, 0.0f, 0.0f) * ModelPacMan;
			}
			else
				Current_PacMan_X += -0.01;
		}
		else
			Current_PacMan_X += -0.01;

		for (int i = 0; i < 84; i += 2)
		{
			float Currant_X = Current_PacMan_X;
			float Currant_Z = Current_PacMan_Z;

			if (Currant_Z == (float)-0.04 || Currant_Z == (float)-0.06)
				Currant_Z = -0.05;

			if (Currant_X == Dots[i] && Currant_Z == Dots[i + 1])
			{
				Dots[i] = -1;
				Dots[i + 1] = -1;
				PacMan_Scour++;
				break;
			}
		}
		if (Personal_Camera == 1)
			Cam.Reset(Current_PacMan_X, 0.6 + Zoom, Current_PacMan_Z + Zoom, Current_PacMan_X, 0, Current_PacMan_Z - 0.4, 0, 1, 0);
		break;

	case GLFW_KEY_A:
		Current_PacMan_X += -0.01;
		if ((!(Current_PacMan_Z > -0.85 && Current_PacMan_Z < -0.06)) && (!(Current_PacMan_Z > -0.04 && Current_PacMan_Z < 0.85))) {
			if (Current_PacMan_X >= -0.85) {
				ModelPacMan = glm::translate(-0.01f, 0.0f, 0.0f) * ModelPacMan;
			}
			else
				Current_PacMan_X += 0.01;
		}
		else
			Current_PacMan_X += 0.01;

		for (int i = 0; i < 84; i += 2)
		{
			float Currant_X = Current_PacMan_X;
			float Currant_Z = Current_PacMan_Z;

			if (Currant_Z == (float)-0.04 || Currant_Z == (float)-0.06)
				Currant_Z = -0.05;

			if (Currant_X == Dots[i] && Currant_Z == Dots[i + 1])
			{
				Dots[i] = -1;
				Dots[i + 1] = -1;
				PacMan_Scour++;
				break;
			}
		}
		if (Personal_Camera == 1)
			Cam.Reset(Current_PacMan_X, 0.6 + Zoom, Current_PacMan_Z + Zoom, Current_PacMan_X, 0, Current_PacMan_Z - 0.4, 0, 1, 0);
		break;

	case GLFW_KEY_P:
		Personal_Camera = 1;
		Cam.Reset(Current_PacMan_X, 0.6 + Zoom, Current_PacMan_Z + Zoom, Current_PacMan_X, 0, Current_PacMan_Z - 0.4, 0, 1, 0);
		break;

	case GLFW_KEY_F1:
		if (Personal_Camera == 1)
		{
			Zoom += 0.1;
			Cam.Reset(Current_PacMan_X, 0.6 + Zoom, Current_PacMan_Z + Zoom, Current_PacMan_X, 0, Current_PacMan_Z - 0.4, 0, 1, 0);
		}
		break;

	case GLFW_KEY_F2:
		if (Personal_Camera == 1)
		{
			Zoom -= 0.1;
			Cam.Reset(Current_PacMan_X, 0.6 + Zoom, Current_PacMan_Z + Zoom, Current_PacMan_X, 0, Current_PacMan_Z - 0.4, 0, 1, 0);
		}
		break;

	default:
		break;
	}

	Cam.UpdateViewMatrix();
}

void Renderer::HandleMouseClick(double X, double Y) {

}

void Renderer::Cleanup()
{
	glDeleteBuffers(1, &vertexBufferID);
	glDeleteBuffers(1, &myIndicesBufferID);
	glDeleteVertexArrays(1, &vertexArrayID); //the one we didn't used (will be discussed later)
	glDeleteProgram(programID);
}

void Renderer::Model_Border()
{
	/*
	_______________________
	|  _______    _______   |
	| |       |	 |       |  |
	| |  Q3   |	 |  Q4   |  |
	| |_______|	 |_______|  |
	|  _______    _______   |
	| |       |	 |       |	|
	| |  Q1   |	 |  Q2   |	|
	| |_______|	 |_______|	|
	|_______________________|

	*/

	Model_Out_Left = glm::translate(-0.95f, +0.0f, +0.0f) * glm::scale(+00.5f, +0.50f, +10.0f);
	Model_Out_Top = glm::translate(+0.0f, +0.0f, -0.95f) * glm::scale(+10.0f, +0.50f, +00.5f);
	Model_Out_Right = glm::translate(+0.95f, +0.0f, +0.0f) * glm::scale(+00.5f, +0.50f, +10.0f);
	Model_Out_Down = glm::translate(+0.0f, +0.0f, +0.95f) * glm::scale(+10.0f, +0.50f, +00.5f);


	Model_Q1_Left = glm::translate(-0.75f, +0.0f, +0.40f) * glm::scale(+00.5f, +0.50f, +3.9f);
	Model_Q1_Right = glm::translate(-0.12f, +0.0f, +0.40f) * glm::scale(+00.5f, +0.50f, +3.9f);
	Model_Q1_Down = glm::translate(-0.45f, +0.0f, +0.74f) * glm::scale(+3.5f, +0.50f, +0.5f);
	Model_Q1_Top = glm::translate(-0.45f, +0.0f, +0.06f) * glm::scale(+3.5f, +0.50f, +0.5f);


	Model_Q2_Left = glm::translate(+0.12f, +0.0f, +0.40f) * glm::scale(+00.5f, +0.50f, +3.9f);
	Model_Q2_Right = glm::translate(+0.75f, +0.0f, +0.40f) * glm::scale(+00.5f, +0.50f, +3.9f);
	Model_Q2_Down = glm::translate(+0.45f, +0.0f, +0.74f) * glm::scale(+3.5f, +0.50f, +0.5f);
	Model_Q2_Top = glm::translate(+0.45f, +0.0f, +0.06f) * glm::scale(+3.5f, +0.50f, +0.5f);

	Model_Q3_Left = glm::translate(-0.75f, +0.0f, -0.45f) * glm::scale(+00.5f, +0.50f, +3.4f);
	Model_Q3_Right = glm::translate(-0.12f, +0.0f, -0.45f) * glm::scale(+00.5f, +0.50f, +3.4f);
	Model_Q3_Down = glm::translate(-0.45f, +0.0f, -0.74f) * glm::scale(+3.5f, +0.50f, +0.5f);
	Model_Q3_Top = glm::translate(-0.45f, +0.0f, -0.16f) * glm::scale(+3.5f, +0.50f, +0.5f);

	Model_Q4_Left = glm::translate(+0.75f, +0.0f, -0.45f) * glm::scale(+00.5f, +0.50f, +3.4f);
	Model_Q4_Right = glm::translate(+0.12f, +0.0f, -0.45f) * glm::scale(+00.5f, +0.50f, +3.4f);
	Model_Q4_Down = glm::translate(+0.45f, +0.0f, -0.74f) * glm::scale(+3.5f, +0.50f, +0.5f);
	Model_Q4_Top = glm::translate(+0.45f, +0.0f, -0.16f) * glm::scale(+3.5f, +0.50f, +0.5f);
}

void Renderer::MVP_Border()
{
	MVP_Out_Left = Cam.GetProjectionMatrix() * Cam.GetViewMatrix() * Model_Out_Left;
	MVP_Out_Top = Cam.GetProjectionMatrix() * Cam.GetViewMatrix() * Model_Out_Top;
	MVP_Out_Right = Cam.GetProjectionMatrix() * Cam.GetViewMatrix() * Model_Out_Right;
	MVP_Out_Down = Cam.GetProjectionMatrix() * Cam.GetViewMatrix() * Model_Out_Down;

	MVP_Q1_Left = Cam.GetProjectionMatrix() * Cam.GetViewMatrix() * Model_Q1_Left;
	MVP_Q1_Right = Cam.GetProjectionMatrix() * Cam.GetViewMatrix() * Model_Q1_Right;
	MVP_Q1_Down = Cam.GetProjectionMatrix() * Cam.GetViewMatrix() * Model_Q1_Down;
	MVP_Q1_Top = Cam.GetProjectionMatrix() * Cam.GetViewMatrix() * Model_Q1_Top;

	MVP_Q2_Left = Cam.GetProjectionMatrix() * Cam.GetViewMatrix() * Model_Q2_Left;
	MVP_Q2_Right = Cam.GetProjectionMatrix() * Cam.GetViewMatrix() * Model_Q2_Right;
	MVP_Q2_Down = Cam.GetProjectionMatrix() * Cam.GetViewMatrix() * Model_Q2_Down;
	MVP_Q2_Top = Cam.GetProjectionMatrix() * Cam.GetViewMatrix() * Model_Q2_Top;

	MVP_Q3_Left = Cam.GetProjectionMatrix() * Cam.GetViewMatrix() * Model_Q3_Left;
	MVP_Q3_Right = Cam.GetProjectionMatrix() * Cam.GetViewMatrix() * Model_Q3_Right;
	MVP_Q3_Down = Cam.GetProjectionMatrix() * Cam.GetViewMatrix() * Model_Q3_Down;
	MVP_Q3_Top = Cam.GetProjectionMatrix() * Cam.GetViewMatrix() * Model_Q3_Top;

	MVP_Q4_Left = Cam.GetProjectionMatrix() * Cam.GetViewMatrix() * Model_Q4_Left;
	MVP_Q4_Right = Cam.GetProjectionMatrix() * Cam.GetViewMatrix() * Model_Q4_Right;
	MVP_Q4_Down = Cam.GetProjectionMatrix() * Cam.GetViewMatrix() * Model_Q4_Down;
	MVP_Q4_Top = Cam.GetProjectionMatrix() * Cam.GetViewMatrix() * Model_Q4_Top;
}

void Renderer::Draw_Border()
{
	BorderFront->Bind();
	mRenderingMode = 1;
	glUniform1i(mRenderingModeID, mRenderingMode);
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_SHORT, 0);

	BorderSide->Bind();
	mRenderingMode = 1;
	glUniform1i(mRenderingModeID, mRenderingMode);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (const void*)12);
}

void Renderer::Blinky_Moveing()
{
	if (Blinky_Counter == 3200)
		Blinky_Counter = 0;
	else if (Blinky_Counter < 1600)
	{
		Blinky_Z += 0.001;
		Current_Blinky_Z += 0.001;
		ModelBlinky = glm::translate(+0.85f, +0.0f, Blinky_Z) * glm::scale(+0.5f, +0.50f, +0.50f);
		Blinky_Counter++;
	}
	else if (Blinky_Counter >= 1600)
	{
		Blinky_Z -= 0.001;
		Current_Blinky_Z += -0.001;
		ModelBlinky = glm::translate(+0.85f, +0.0f, Blinky_Z) * glm::scale(+0.5f, +0.50f, +0.50f);
		Blinky_Counter++;
	}
}

void Renderer::Clyde_Moveing()
{
	if (Clyde_Counter == 3200)
		Clyde_Counter = 0;
	else if (Clyde_Counter < 1600)
	{
		Clyde_X += 0.001;
		Current_Clyde_X += 0.001;
		ModelClyde = glm::translate(Clyde_X, +0.0f, -0.85f) * glm::scale(+0.5f, +0.50f, +0.50f);
		Clyde_Counter++;
	}
	else if (Clyde_Counter >= 1600)
	{
		Clyde_X -= 0.001;
		Current_Clyde_X += -0.001;
		ModelClyde = glm::translate(Clyde_X, +0.0f, -0.85f) * glm::scale(+0.5f, +0.50f, +0.50f);
		Clyde_Counter++;
	}
}

void Renderer::Linky_Moveing()
{
	if (First_Time == 1)
	{
		Linky_X -= 0.001;
		Current_Inky_X += -0.001;
		ModelLinky = glm::translate(Linky_X, +0.0f, -0.05f) * glm::scale(+0.5f, +0.50f, +0.50f);
		Linky_Counter++;

		if (Linky_Counter == 800)
		{
			Linky_Counter = 0;
			First_Time = 0;
		}
	}
	else
	{
		if (Linky_Counter == 3200)
			Linky_Counter = 0;
		else if (Linky_Counter < 1600)
		{
			Linky_X += 0.001;
			Current_Inky_X += 0.001;
			ModelLinky = glm::translate(Linky_X, +0.0f, -0.05f) * glm::scale(+0.5f, +0.50f, +0.50f);
			Linky_Counter++;
		}
		else if (Linky_Counter >= 1600)
		{
			Linky_X -= 0.001;
			Current_Inky_X += -0.001;
			ModelLinky = glm::translate(Linky_X, +0.0f, -0.05f) * glm::scale(+0.5f, +0.50f, +0.50f);
			Linky_Counter++;
		}
	}
}

void Renderer::CheckDynamicCollision()
{
	if (Current_PacMan_Z >= (Current_Inky_Z - 0.1) && Current_PacMan_Z <= (Current_Inky_Z + 0.1)) //Check if the PacMan in the same z-axis range of the Inky. 
	{
		if ((trunc(Current_PacMan_X * 100) <= trunc((Current_Inky_X + 0.1) * 100)) &&
			(trunc(Current_PacMan_X * 100) >= trunc((Current_Inky_X - 0.1) * 100))) //Check if the PacMan touch the Inky.
			exit(0); //Comment this line to continue the game.
	}
	//Why +0.1 or -0.1? cause this coordinates of the center of the character and we need to get the edge of cube not the center.

	if (Current_PacMan_Z >= (Current_Clyde_Z - 0.1) && Current_PacMan_Z <= (Current_Clyde_Z + 0.1))
	{
		if ((trunc(Current_PacMan_X * 100) <= trunc((Current_Clyde_X + 0.1) * 100)) &&
			(trunc(Current_PacMan_X * 100) >= trunc((Current_Clyde_X - 0.1) * 100)))
			exit(0);
	}

	if (Current_PacMan_X >= (Current_Blinky_X - 0.1) && Current_PacMan_X <= (Current_Blinky_X + 0.1))
	{
		if ((trunc(Current_PacMan_Z * 100) <= trunc((Current_Blinky_Z + 0.1) * 100)) &&
			(trunc(Current_PacMan_Z * 100) >= trunc((Current_Blinky_Z - 0.1) * 100)))
			exit(0);
	}
}