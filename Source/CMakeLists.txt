include_directories (${OPENGL_INCLUDE_DIR} ${SDL_INCLUDE_DIR}
  ${VORBIS_INCLUDE_DIR} ${OPENAL_INCLUDE_DIR} ${ALUT_INCLUDE_DIR})

add_executable (blackshades
  Button.cpp
  Camera.cpp
  Config.cpp
  Decals.cpp
  Fog.cpp
  Font.cpp
  Frustum.cpp
  Game.cpp
  GameDraw.cpp
  GameInitDispose.cpp
  GameLoop.cpp
  GameTick.cpp
  Globals.cpp
  Image.cpp
  Label.cpp
  MacInput.cpp
  Main.cpp
  Maths.cpp
  Models.cpp
  Person.cpp
  Quaternions.cpp
  SDL_funcs.cpp
  Serialize.cpp
  Skeleton.cpp
  Sprites.cpp
  Support.cpp
  TGALoader.cpp
  Text.cpp
  Weapon.cpp
  Window.cpp
  Net/Client.cpp
  Net/NetCommon.cpp
  Net/Server.cpp)

target_link_libraries (blackshades
  ${OPENAL_LIBRARY}
  ${ALUT_LIBRARY}
  OpenGL::GL OpenGL::GLU
  ${SDL_LIBRARY}
  ${VORBIS_LIBRARIES})
