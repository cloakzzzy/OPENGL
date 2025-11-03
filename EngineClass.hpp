#pragma once

class Torus;

namespace Engine {
	class Engine {
		friend class Torus;
		inline static int u_VAO;
	public:

		static void Initialize();
		static void Render();
	};

	
}