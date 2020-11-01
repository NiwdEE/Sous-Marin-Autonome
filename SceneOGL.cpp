#include "SceneOGL.hpp"



using namespace glm;


SceneOGL::SceneOGL(std::string WinTitle)
{
	Config WinCfg("configs/Window.txt");
	WinCfg.Load();

	mWinTitle = WinTitle;
	mWinWidth = strtoint(WinCfg.getValue("WindowWidth"));
	mWinHeight = strtoint(WinCfg.getValue("WindowHeight"));

	mWindow = nullptr;
}

SceneOGL::~SceneOGL()
{
    SDL_GL_DeleteContext(mContext);
    SDL_DestroyWindow(mWindow);
    TTF_Quit();
    SDL_Quit();
}

bool SceneOGL::InitSDL()
{

    // Initialisation de la SDL
	
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        std::cout << "Erreur lors de l'initialisation de la SDL : " << SDL_GetError() << std::endl;
        SDL_Quit();
		
        return false;
    }
	
	TTF_Init();
	
    // Version d'OpenGL
	
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	
	
    // Double Buffer
	
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	
	
    // Création de la fenêtre
	
    mWindow = SDL_CreateWindow(mWinTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, mWinWidth, mWinHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

    if(mWindow == 0)
    {
        std::cout << "Erreur lors de la creation de la fenetre : " << SDL_GetError() << std::endl;
        SDL_Quit();

        return false;
    }


    // Création du contexte OpenGL

    mContext = SDL_GL_CreateContext(mWindow);

    if(mContext == 0)
    {
        std::cout << SDL_GetError() << std::endl;
        SDL_DestroyWindow(mWindow);
        SDL_Quit();

        return false;
    }

    SDL_GL_SetSwapInterval(0);

    SDLNet_Init();

    return true;
}


bool SceneOGL::InitGL()
{

    #ifdef WIN32

        // On initialise GLEW
		glewExperimental = GL_TRUE;
        GLenum initialisationGLEW( glewInit() );

        if(initialisationGLEW != GLEW_OK)
        {
            // On affiche l'erreur grâce à la fonction : glewGetErrorString(GLenum code)

            std::cout << "Erreur d'initialisation de GLEW : " << glewGetErrorString(initialisationGLEW) << std::endl;


            // On quitte la SDL

            SDL_GL_DeleteContext(mContext);
            SDL_DestroyWindow(mWindow);
            SDL_Quit();

            return false;
        }

        glEnable(GL_DEPTH_TEST);
     	glDepthFunc(GL_LESS);

     	//glEnable(GL_TEXTURE_2D);

    #endif


    return true;
}

void DrawHUD(void)
{

}

void SceneOGL::MainLoop()
{

    unsigned int frameRate (1000 / 1000);
    Uint32 LoopStart(0), LoopDuration(0);

    srand(time(NULL));

    FrameBuffer framebuffer(10000, 5000);
    framebuffer.Load();
    mat4 projectionFBO;
    projectionFBO = perspective(70.0, (double)framebuffer.getWidth()/framebuffer.getHeight(), 0.1, 200.0);
    mat4 modelviewFBO;
    modelviewFBO = mat4(1.0);

    mat4 projection;
    mat4 modelview;

    projection = perspective(70.0, (double) 1920 / 1080, 0.01, 200.0);
    modelview = mat4(1.0);
    Camera camera(vec3(3,3,3), vec3(0,0,0), vec3(0,1,0));

    Slab ground(vec3(0,-0.0001,0), vec3(100,1,100), "Shaders/texture.vert", "Shaders/texture.frag");
    ground.setCenterPos(REL, vec3(0.5,1,0.5));
    ground.setTexture("src/Textures/grass.jpg");
    ground.setTextureReps(vec3(100,100,100));
    ground.Load();

    Slab cube(vec3(1,0,1), vec3(1,1,1), "Shaders/texture.vert", "Shaders/texture.frag");
    cube.setCenterPos(REL, vec3(0.5,0,0.5));
    cube.setTexture("src/Textures/box2.jpg");
    cube.setTextureReps(vec3(1,1,1));
    cube.Load();

    Slab skybox(vec3(0,0,0), vec3(101,101,101), "Shaders/texture.vert", "Shaders/texture.frag");
    skybox.setCenterPos(REL, vec3(0.5,0.5,0.5));
    skybox.setTexture("src/Textures/box.jpg");
    skybox.setTextureReps(vec3(1,1,1));
    skybox.Load();

    mInput.TrapCursor(true);
    mInput.ShowCursor(false);
    //SDL_SetWindowGrab(mWindow, SDL_TRUE);

    // Boucle principale

    // Shader

    Shader shaderTexture("Shaders/texture.vert", "Shaders/texture.frag");
    shaderTexture.Load();

    Slab testrect(vec3(10,10,0), vec3(5,0,5), "Shaders/texture.vert", "Shaders/texture.frag");
    testrect.Load();

    Slab BrickWall(vec3(0, 0, 20), vec3(5, 20, 5), "Shaders/texture.vert", "Shaders/texture.frag");
    BrickWall.setTexture("src/Textures/wood1.jpg");
    BrickWall.Load();


    int frames = 0;
    Uint32 lastfrprint = SDL_GetTicks();

    Player player(1, "Test", 1.8, camera);
    //

    int MoveDir = 0;

    Object3D marex("Shaders/StandardShading.vertexshader", "Shaders/StandardShading.fragmentshader", "src/3dobj/turbodoggo/doggo.obj");
    marex.Load();

    HUD ColorPicker;
    ColorPicker.AddSquare(0, 0, 0.5, 0.5, V, vec4(0, 0, 0, 255), vec4(255, 255, 255, 255));

	vec3 ColorPickerColor(0, 0, 255);
	vec3 FinalColorPickerColor(0, 0, 0);

    ColorPicker.AddSquare(0, 0, 0.5, 0.5, vec4(ColorPickerColor, 0), vec4(ColorPickerColor, 0), vec4(ColorPickerColor, 255), vec4(ColorPickerColor, 0), "Picker");

    ColorPicker.AddSquare( 0, 0.5, 1.0f/6, 0.5, H, vec4(255, 0, 0, 255), vec4(255, 255, 0, 255));
    ColorPicker.AddSquare(1.0f/6, 0.5, 1.0f/6, 0.5, H, vec4(255, 255, 0, 255), vec4(0, 255, 0, 255));
    ColorPicker.AddSquare(2.0f/6, 0.5, 1.0f/6, 0.5, H, vec4(0, 255, 0, 255), vec4(0, 255, 255, 255));
    ColorPicker.AddSquare(3.0f/6, 0.5, 1.0f/6, 0.5, H, vec4(0, 255, 255, 255), vec4(0, 0, 255, 255));
    ColorPicker.AddSquare(4.0f/6, 0.5, 1.0f/6, 0.5, H, vec4(0, 0, 255, 255), vec4(255, 0, 255, 255));
    ColorPicker.AddSquare(5.0f/6, 0.5, 1.0f/6, 0.5, H, vec4(255, 0, 255, 255), vec4(255, 0, 0, 255));

    ColorPicker.AddSquare(0.5, 0, 0.5, 0.5, vec4(0, 0, 0, 0), "PickedALL");

    ColorPicker.AddSquare(0, 0.5, 0.0075f / (float(mWinWidth)/mWinHeight), 0.0075, vec4(0, 0, 0, 255), "CursorOut");
    ColorPicker.AddSquare(0, 0.5, 0.005f / (float(mWinWidth)/mWinHeight), 0.005, vec4(255, 255, 255, 255), "CursorIn");

    ColorPicker.AddSquare(0, 0.75, 0.0075f / (float(mWinWidth)/mWinHeight), 0.0075, vec4(0, 0, 0, 255), "CursorOut2");
    ColorPicker.AddSquare(0, 0.75, 0.005f / (float(mWinWidth)/mWinHeight), 0.005, vec4(255, 255, 255, 255), "CursorIn2");

    ColorPicker.Load();

    auto RefreshPickedColor = [&](){
    	float p = float((255- ColorPicker.getSquareX("CursorOut")*2*255))*2*ColorPicker.getSquareY("CursorOut")/255;
    	int bg = ColorPicker.getSquareY("CursorOut")*2*255/mWinHeight;
    	ColorPicker.SetSquareColor("PickedALL", vec4( vec3((1-p)*bg) + p*ColorPickerColor ,255));
    };

    RefreshPickedColor();

    bool DisplayMenu = false;

    HUD FPS;
    FPS.AddText(0, 0, "0", 50, vec4(255, 255, 255, 255), "FPS");

    FPS.Load();

    camera.Orientate(0,1);

    player.setPos(vec3(0,0,0));
    player.setVel(vec3(0,0,0));

    Uint32 LastCaseMoved = SDL_GetTicks();

    while(!mInput.Stop())
    {
    	LoopStart = SDL_GetTicks();

    	frames++;

    	cube.setPos(vec3(cube.getPos().x + float(SDL_GetTicks() - LastCaseMoved)/1000,cube.getPos().y,cube.getPos().z));

    	LastCaseMoved = SDL_GetTicks();

    	mInput.UpdateEvents();

    	if(mInput.Stop())
    		break;

    	camera.Orientate(mInput.getMouseRelX(),mInput.getMouseRelY());
        player.Refresh();

        // if(collide(Ray(camera.getPos(),camera.getOrientation(),100),Plane::Infinite(vec3(0,0,0), vec3(0,1,0)) )){
        //     std::cout << "tu regardes le sol !" << std::endl;
        // }	

    	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    	camera.Refresh(modelview);

    	// glViewport(0, 0, mWinWidth, mWinHeight);

    	skybox.Display(projection, modelview);

    	ground.Display(projection, modelview);

    	cube.Display(projection, modelview);

        testrect.Display(projection, modelview);

        //BrickWall.Display(projection, modelview);

        marex.Display(projection, modelview);

        FPS.Display();

        if(DisplayMenu)
        	ColorPicker.Display();

        SDL_GL_SwapWindow(mWindow);

    	if(mInput.IsKeyPressed(SDL_SCANCODE_RIGHT)){
            if(mInput.IsKeyPressed(SDL_SCANCODE_LCTRL))
            	cube.setAngles(vec2(cube.getAngles().x+5,cube.getAngles().y));
            else
            	cube.setPos(vec3(cube.getPos().x+1,cube.getPos().y,cube.getPos().z));
        }
        else if(mInput.IsKeyPressed(SDL_SCANCODE_LEFT)){
        	if(mInput.IsKeyPressed(SDL_SCANCODE_LCTRL))
            	cube.setAngles(vec2(cube.getAngles().x-5,cube.getAngles().y));
            else
            	cube.setPos(vec3(cube.getPos().x-1,cube.getPos().y,cube.getPos().z)) ;
        }

        if(mInput.IsKeyPressed(SDL_SCANCODE_DOWN)){
        	if(mInput.IsKeyPressed(SDL_SCANCODE_LCTRL))
            	cube.setAngles(vec2(cube.getAngles().x,cube.getAngles().y+5));
            else
            	cube.setPos(vec3(cube.getPos().x,cube.getPos().y,cube.getPos().z+1));
        }
        else if(mInput.IsKeyPressed(SDL_SCANCODE_UP)){
        	if(mInput.IsKeyPressed(SDL_SCANCODE_LCTRL))
            	cube.setAngles(vec2(cube.getAngles().x,cube.getAngles().y-5));
            else
            	cube.setPos(vec3(cube.getPos().x,cube.getPos().y,cube.getPos().z-1)) ;
        }

        if(mInput.TreatKey(SDL_SCANCODE_SPACE)){
        	player.Jump(4);
        }

        if(mInput.IsMBPressed(1) && DisplayMenu && mInput.getMouseX() < mWinWidth/2 && mInput.getMouseY() < mWinHeight/2){
        	ColorPicker.SetSquarePos("CursorOut", mInput.getMouseSX(mWinWidth), mInput.getMouseSY(mWinHeight));
        	ColorPicker.SetSquarePos("CursorIn", mInput.getMouseSX(mWinWidth), mInput.getMouseSY(mWinHeight));
        	RefreshPickedColor();
        }

        if(mInput.IsMBPressed(1) && DisplayMenu && mInput.getMouseY() > mWinHeight/2){
        	ColorPicker.SetSquarePos("CursorOut2", mInput.getMouseSX(mWinWidth), mInput.getMouseSY(mWinHeight));
        	ColorPicker.SetSquarePos("CursorIn2", mInput.getMouseSX(mWinWidth), mInput.getMouseSY(mWinHeight));
        	ColorPickerColor.x = CLAMP(0, 255, ((1 - (6*mInput.getMouseSX(mWinWidth) - 1.0f)))*255 ) + CLAMP(0, 255, (6*mInput.getMouseSX(mWinWidth) - 5)*255) ;
        	ColorPickerColor.y = CLAMP(0, 255, (6*mInput.getMouseSX(mWinWidth))*255) - CLAMP(0, 255, (6*mInput.getMouseSX(mWinWidth) - 3)*255);
        	ColorPickerColor.z = CLAMP(0, 255, (6*mInput.getMouseSX(mWinWidth) - 2)*255 ) - CLAMP(0, 255, (6*mInput.getMouseSX(mWinWidth) - 5)*255);
        	ColorPicker.SetSquareColor("Picker", vec4(ColorPickerColor, 0), vec4(ColorPickerColor, 0), vec4(ColorPickerColor, 255), vec4(ColorPickerColor, 0));
        	RefreshPickedColor();
        }

        if (mInput.TreatKey(SDL_SCANCODE_ESCAPE)){
      		DisplayMenu=!DisplayMenu;
         	mInput.ShowCursor(DisplayMenu);
    		mInput.TrapCursor(!DisplayMenu);
    		SDL_WarpMouseInWindow(mWindow,mWinWidth/2,mWinHeight/2);
    		camera.Lock(DisplayMenu);
        }

        if(mInput.IsKeyPressed(SDL_SCANCODE_LCTRL))
        	player.setAcc(vec3(0,10,0));
        else
        	player.setAcc(vec3(0,-9.81,0));

        MoveDir = 0;
        if (mInput.getLastHitKey(2, SDL_SCANCODE_W, SDL_SCANCODE_S) == SDL_SCANCODE_W)
        	MoveDir |= FRONT;
        else if (mInput.getLastHitKey(2, SDL_SCANCODE_W, SDL_SCANCODE_S) == SDL_SCANCODE_S)
        	MoveDir |= BACK;

        if (mInput.getLastHitKey(2, SDL_SCANCODE_A, SDL_SCANCODE_D) == SDL_SCANCODE_A)
	        MoveDir |= RIGHT;
        else if (mInput.getLastHitKey(2, SDL_SCANCODE_A, SDL_SCANCODE_D) == SDL_SCANCODE_D)
        	MoveDir |= LEFT;

        player.setDirection(MoveDir);

        if(mInput.IsMBPressed(3) && player.IsOnGround()){
        	// player.setVel(10.0f * vec3(camera.getOrientation().x, camera.getOrientation().y, camera.getOrientation().z));

        }

        if(cube.getPos().x>50)
        	cube.setPos(vec3(-50,cube.getPos().y,cube.getPos().z));
        else if(cube.getPos().x<-50)
        	cube.setPos(vec3(50,cube.getPos().y,cube.getPos().z));

        if(cube.getPos().z>50)
        	cube.setPos(vec3(cube.getPos().x,cube.getPos().y,-50));
        else if(cube.getPos().z<-50)
        	cube.setPos(vec3(cube.getPos().x,cube.getPos().y,50));

    	if(lastfrprint + 999 < SDL_GetTicks())
    	{
    		//std::cout <<  frames << std::endl;
    		FPS.setText("FPS", std::string(inttostr(frames)), 50, vec4(255, 255, 255, 255));
    		lastfrprint = SDL_GetTicks();
    		frames=0;
    	}

    	LoopDuration = SDL_GetTicks() - LoopStart;
    	if (LoopDuration < frameRate)
    		SDL_Delay(frameRate - LoopDuration);
    }
    
}

Cube::Cube(float taille, std::string const vertexShader, std::string const fragmentShader) : m_shader(vertexShader, fragmentShader)
{
    // Chargement du shader

    m_shader.Load();


    // Division de la taille

    taille /= 2;


    // Vertices temporaires

    float verticesTmp[] = {-taille, -taille, -taille,   taille, -taille, -taille,   taille, taille, -taille,     // Face 1
                           -taille, -taille, -taille,   -taille, taille, -taille,   taille, taille, -taille,     // Face 1

                           taille, -taille, taille,   taille, -taille, -taille,   taille, taille, -taille,       // Face 2
                           taille, -taille, taille,   taille, taille, taille,   taille, taille, -taille,         // Face 2

                           -taille, -taille, taille,   taille, -taille, taille,   taille, -taille, -taille,      // Face 3
                           -taille, -taille, taille,   -taille, -taille, -taille,   taille, -taille, -taille,    // Face 3

                           -taille, -taille, taille,   taille, -taille, taille,   taille, taille, taille,        // Face 4
                           -taille, -taille, taille,   -taille, taille, taille,   taille, taille, taille,        // Face 4

                           -taille, -taille, -taille,   -taille, -taille, taille,   -taille, taille, taille,     // Face 5
                           -taille, -taille, -taille,   -taille, taille, -taille,   -taille, taille, taille,     // Face 5

                           -taille, taille, taille,   taille, taille, taille,   taille, taille, -taille,         // Face 6
                           -taille, taille, taille,   -taille, taille, -taille,   taille, taille, -taille};      // Face 6

        // Triangle 2


    // Couleurs temporaires

    float couleursTmp[] = {1.0, 0.0, 0.0,   1.0, 0.0, 0.0,   1.0, 0.0, 0.0,           // Face 1
                           1.0, 0.0, 0.0,   1.0, 0.0, 0.0,   1.0, 0.0, 0.0,           // Face 1

                           0.0, 1.0, 0.0,   0.0, 1.0, 0.0,   0.0, 1.0, 0.0,           // Face 2
                           0.0, 1.0, 0.0,   0.0, 1.0, 0.0,   0.0, 1.0, 0.0,           // Face 2

                           0.0, 0.0, 1.0,   0.0, 0.0, 1.0,   0.0, 0.0, 1.0,           // Face 3
                           0.0, 0.0, 1.0,   0.0, 0.0, 1.0,   0.0, 0.0, 1.0,           // Face 3

                           1.0, 0.0, 0.0,   1.0, 0.0, 0.0,   1.0, 0.0, 0.0,           // Face 4
                           1.0, 0.0, 0.0,   1.0, 0.0, 0.0,   1.0, 0.0, 0.0,           // Face 4

                           0.0, 1.0, 0.0,   0.0, 1.0, 0.0,   0.0, 1.0, 0.0,           // Face 5
                           0.0, 1.0, 0.0,   0.0, 1.0, 0.0,   0.0, 1.0, 0.0,           // Face 5

                           0.0, 0.0, 1.0,   0.0, 0.0, 1.0,   0.0, 0.0, 1.0,           // Face 6
                           0.0, 0.0, 1.0,   0.0, 0.0, 1.0,   0.0, 0.0, 1.0};          // Face 6


    // Copie des valeurs dans les tableaux finaux

    for(int i(0); i < 108; i++)
    {
        m_vertices[i] = verticesTmp[i];
        m_couleurs[i] = couleursTmp[i];
    }
}

void Cube::Load()
{
	if(glIsBuffer(mVboID))
		glDeleteBuffers(1, &mVboID);

	glGenBuffers(1, &mVboID);

	glBindBuffer(GL_ARRAY_BUFFER, mVboID);

		glBufferData(GL_ARRAY_BUFFER, 108*2*sizeof(float), 0, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, 108*sizeof(float), m_vertices);
		glBufferSubData(GL_ARRAY_BUFFER, 108*sizeof(float), 108*sizeof(float), m_couleurs);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	if(glIsVertexArray(mVaoID))
		glDeleteVertexArrays(1, &mVaoID);

	glGenVertexArrays(1, &mVaoID);

	glBindVertexArray(mVaoID);

		glBindBuffer(GL_ARRAY_BUFFER,mVboID);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(108*sizeof(float)));
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER,0);

	glBindVertexArray(0);

}

Cube::~Cube(){

}

void Cube::afficher(glm::mat4 &projection, glm::mat4 &modelview)
{
    // Activation du shader

    Texture box("src/Textures/box.jpg");
    box.Load();

    glUseProgram(m_shader.getProgramID());

       glBindVertexArray(mVaoID);

	        glUniformMatrix4fv(glGetUniformLocation(m_shader.getProgramID(), "projection"), 1, GL_FALSE, value_ptr(projection));
	        glUniformMatrix4fv(glGetUniformLocation(m_shader.getProgramID(), "modelview"), 1, GL_FALSE, value_ptr(modelview));

	        glBindTexture(GL_TEXTURE_2D, box.getID());

	        glDrawArrays(GL_TRIANGLES, 0, 36);

	        glBindTexture(GL_TEXTURE_2D, 0);

       glBindVertexArray(0);

    glUseProgram(0);
}

Ground::Ground(float taille, float reps, std::string const vertexShader, std::string const fragmentShader, std::string texturepath) : mShader(vertexShader, fragmentShader), mTexturePath(texturepath)
{
    // Chargement du shader

    mShader.Load();

    taille/=2;

    // Vertices temporaires

    float verticesTmp[] = {-taille, 0, taille,   taille, 0, taille,   taille, 0, -taille,         // Face 6
                           -taille, 0, taille,   -taille, 0, -taille,   taille, 0, -taille};




    // Couleurs temporaires

    float textureTmp[] = {0, 0,   reps, 0,   reps, reps,
                           0, 0,   0, reps,   reps, reps};          // Face 6


    // Copie des valeurs dans les tableaux finaux

    for(int i(0); i < 18; i++)
    {
        mVerticeGround[i] = verticesTmp[i];
    }

    for (int i = 0; i < 12; ++i)
    {
        mVerticeTexture[i] = textureTmp[i];
    }
}

Ground::~Ground()
{

}

void Ground::afficher(glm::mat4 &projection, glm::mat4 &modelview)
{
     glUseProgram(mShader.getProgramID());

        Texture textr(mTexturePath);
        textr.Load();

        // Envoi des vertices

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, mVerticeGround);
        glEnableVertexAttribArray(0);


        // Envoi de la couleur

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, mVerticeTexture);
        glEnableVertexAttribArray(2);

    

        // Envoi des matrices

        glUniformMatrix4fv(glGetUniformLocation(mShader.getProgramID(), "projection"), 1, GL_FALSE, value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(mShader.getProgramID(), "modelview"), 1, GL_FALSE, value_ptr(modelview));

        glBindTexture(GL_TEXTURE_2D, textr.getID());

        // Rendu

        glDrawArrays(GL_TRIANGLES, 0, 6);

        glBindTexture(GL_TEXTURE_2D, 0);

        // Désactivation des tableaux

        glDisableVertexAttribArray(2);
        glDisableVertexAttribArray(0);


    // Désactivation du shader

    glUseProgram(0);
}