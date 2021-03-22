#pragma once
#ifndef SFML_DYNAMIC
	#define SFML_DYNAMIC
#endif
#ifdef _DEBUG
	#pragma comment(lib, "lib/sfml-graphics-d.lib")
	#pragma comment(lib, "lib/sfml-audio-d.lib")
	#pragma comment(lib, "lib/sfml-network-d.lib")
	#pragma comment(lib, "lib/sfml-system-d.lib")
	#pragma comment(lib, "lib/sfml-window-d.lib")
#else
	#pragma comment(lib, "lib/sfml-graphics.lib")
	#pragma comment(lib, "lib/sfml-audio.lib")
	#pragma comment(lib, "lib/sfml-network.lib")
	#pragma comment(lib, "lib/sfml-system.lib")
	#pragma comment(lib, "lib/sfml-window.lib")
#endif

#include <SFML/Graphics.hpp>
#include "Math.h"
#include "include/my/binary.h"

//Vailder3D namespace
class v3d
{
public:
	typedef std::vector<std::vector<int>> matrix;

	static matrix generateMatrix(const unsigned int sizeX, const unsigned int sizeY)
	{
		return matrix(sizeX, std::vector<int>(sizeY, 0));
	}

	//Обычно нужен для хранения информации о 4 углах
	template<typename T>
	class quad
	{
	public:
		T a;
		T b;
		T c;
		T d;

		quad() :
			a(T()),
			b(T()),
			c(T()),
			d(T())
		{

		}

		quad(const T a, const T b, const T c, const T d) :
			a(a),
			b(b),
			c(c),
			d(d)
		{

		}
	};
	
	//clock, timer
	template<typename Time>
	class clock
	{
		static_assert(std::is_same<Time, float>::value || std::is_same<Time, double>::value || std::is_same<Time, long double>::value,
			"template's typename of clock are not <float> or <double> or <long double>");
	protected:
		//delta time
		Time d_time_ = 0;

	public:

		static Time toMilliseconds(const Time sec)
		{
			return sec * 1000;
		}

		static Time toSeconds(const Time ms)
		{
			return ms * 0.001;
		}

		static Time getGlobalTime_sec()
		{
			return static_cast<Time>(std::clock()) / static_cast<Time>(CLOCKS_PER_SEC);
		}

		static Time getGlobalTime_ms()
		{
			return getGlobalTime_sec() * 1000;
		}

		Time getTime_sec()  const
		{
			return getGlobalTime_sec() - d_time_;
		}

		Time getTime_ms() const
		{
			return  (getGlobalTime_sec() - d_time_) * 1000;
		}

		bool restart_clock()
		{
			d_time_ = getTime_sec();
			return true;
		}
	};

	//Генератор лабиринта
	template<unsigned int SizeX, unsigned int SizeY, unsigned Seed>
	class maze
	{
	private:
		//Варианты поворота
		enum class Rot
		{
			LEFT,
			RIGHT,
			DOWN,
			UP,
			RotNull
		};

		//Класс для функции, ищущей варианты поворота
		class ret
		{
		public:
			Rot rot;
			sf::Vector2<int> pos;
			ret() :
				rot(Rot::RotNull),
				pos(sf::Vector2<int>())
			{

			}
			ret(const Rot rot, const sf::Vector2<int> pos) :
				rot(rot),
				pos(pos)
			{

			}
		};

		//Эта функция находит возможный вариант поворота лабиринта в определённую сторону
		//Берём адрес для ускорения генерации
		static ret fnd(matrix& matrix, std::vector<sf::Vector2<int>>& maze_way_vec)
		{
			int sdpY(0);
			int sdpX(0);
			for (size_t i = 0; i < maze_way_vec.size(); i++)
			{
				//0
				sdpY = maze_way_vec[i].y - 1;
				sdpX = maze_way_vec[i].x;
				if (sdpY > 0)
				{
					if (sdpY < SizeY - 1)
					{
						if (sdpX > 0)
						{
							if (sdpX < SizeX - 1)
							{
								if (matrix[sdpX][sdpY] != 1)
								{
									if (matrix[sdpX][sdpY - 1] != 1)
									{
										if (matrix[sdpX + 1][sdpY] != 1)
										{
											if (matrix[sdpX - 1][sdpY] != 1)
											{
												return ret(Rot::UP, sf::Vector2<int>(sdpX, sdpY));
											}
										}
									}
								}
							}
						}
					}
				}
				//1
				sdpY = maze_way_vec[i].y + 1;
				sdpX = maze_way_vec[i].x;
				if (sdpY > 0)
				{
					if (sdpY < SizeY - 1)
					{
						if (sdpX > 0)
						{
							if (sdpX < SizeX - 1)
							{
								if (matrix[sdpX][sdpY] != 1)
								{
									if (matrix[sdpX][sdpY + 1] != 1)
									{
										if (matrix[sdpX + 1][sdpY] != 1)
										{
											if (matrix[sdpX - 1][sdpY] != 1)
											{
												return ret(Rot::DOWN, sf::Vector2<int>(sdpX, sdpY));
											}
										}
									}
								}
							}
						}
					}
				}
				//2
				sdpY = maze_way_vec[i].y;
				sdpX = maze_way_vec[i].x - 1;
				if (sdpY > 0)
				{
					if (sdpY < SizeY - 1)
					{
						if (sdpX > 0)
						{
							if (sdpX < SizeX - 1)
							{
								if (matrix[sdpX][sdpY] != 1)
								{
									if (matrix[sdpX][sdpY - 1] != 1)
									{
										if (matrix[sdpX][sdpY + 1] != 1)
										{
											if (matrix[sdpX - 1][sdpY] != 1)
											{
												return ret(Rot::LEFT, sf::Vector2<int>(sdpX, sdpY));
											}
										}
									}
								}
							}
						}
					}
				}
				//3
				sdpY = maze_way_vec[i].y;
				sdpX = maze_way_vec[i].x + 1;
				if (sdpY > 0)
				{
					if (sdpY < SizeY - 1)
					{
						if (sdpX > 0)
						{
							if (sdpX < SizeX - 1)
							{
								if (matrix[sdpX][sdpY] != 1)
								{
									if (matrix[sdpX][sdpY - 1] != 1)
									{
										if (matrix[sdpX][sdpY + 1] != 1)
										{
											if (matrix[sdpX + 1][sdpY] != 1)
											{
												return ret(Rot::RIGHT, sf::Vector2<int>(sdpX, sdpY));
											}
										}
									}
								}
							}
						}
					}
				}
			}
			return ret();
		}
	public:
		//Функция, возвращающая лабиринт
		static matrix generate()
		{
			unsigned __int8 seed[64] = {};
			class matrix matrix = generateMatrix(SizeX, SizeY);
			sf::Vector2<int> pos = sf::Vector2<int>(static_cast<int>(SizeX / 2), static_cast<int>(SizeY / 2));
			std::vector<sf::Vector2<int>> maze_way_vec;

			for (auto i = 0; i < 64; i++)
			{
				seed[i] = static_cast<char>(math::rand(0, 3, Seed / (i + 1)));
			}

			maze_way_vec.push_back(pos);
			matrix[pos.x][pos.y] = 1;

			const int for_num = SizeX * SizeY;
			for (size_t l = 0; l < for_num; l++)
			{
				const class ret fnd_ret = fnd(matrix, maze_way_vec);
				if (fnd_ret.rot == Rot::RotNull)
				{
					return matrix;
				}
				pos = fnd_ret.pos;
				matrix[pos.x][pos.y] = 1;
				maze_way_vec.emplace_back(pos.x, pos.y);

				unsigned maze_time = 0;

				for (unsigned i = maze_time; i < 4; i++)
				{
					if (seed[i] == 0)
					{
						if (pos.y - 1 > 0)
						{
							if (matrix[pos.x][pos.y - 1] != 1)
							{
								const int mpY = pos.y - 1;
								const int mpX = pos.x;
								if (matrix[mpX][mpY - 1] != 1)
								{
									if (matrix[mpX + 1][mpY] != 1)
									{
										if (matrix[mpX - 1][mpY] != 1)
										{
											pos.y -= 1;
											matrix[pos.x][pos.y] = 1;
											maze_way_vec.emplace_back(pos.x, pos.y);
										}
									}
								}
							}
						}
					}
					else if (seed[i] == 1)
					{
						if (pos.y + 1 < SizeY - 1)
						{
							if (matrix[pos.y + 1][pos.x] != 1)
							{
								const int mpY = pos.y + 1;
								const int mpX = pos.x;
								if (matrix[mpX + 1][mpY] != 1)
								{
									if (matrix[mpX - 1][mpY] != 1)
									{
										if (matrix[mpX][mpY + 1] != 1)
										{
											pos.y += 1;
											matrix[pos.x][pos.y] = 1;
											maze_way_vec.emplace_back(pos.x, pos.y);
										}
									}
								}
							}
						}
					}
					else if (seed[i] == 2)
					{
						if (pos.x - 1 > 0)
						{
							if (matrix[pos.x - 1][pos.y] != 1)
							{
								const int mpY = pos.y;
								const int mpX = pos.x - 1;
								if (matrix[mpX][mpY - 1] != 1)
								{
									if (matrix[mpX][mpY + 1] != 1)
									{
										if (matrix[mpX - 1][mpY] != 1)
										{
											pos.x -= 1;
											matrix[pos.x][pos.y] = 1;
											maze_way_vec.emplace_back(pos.x, pos.y);
										}
									}
								}
							}
						}
					}
					else if (seed[i] == 3)
					{
						if (pos.x + 1 < SizeX - 1)
						{
							if (matrix[pos.x][pos.y] != 1)
							{
								const int mpY = pos.y;
								const int mpX = pos.x + 1;
								if (matrix[mpX][mpY + 1] != 1)
								{
									if (matrix[mpX][mpY - 1] != 1)
									{
										if (matrix[mpX + 1][mpY] != 1)
										{
											pos.x += 1;
											matrix[pos.x][pos.y] = 1;
											maze_way_vec.emplace_back(pos.x, pos.y);
										}
									}
								}
							}
						}
					}
					maze_time++;
					if (maze_time == 64)
					{
						maze_time = 0;
					}
				}
			}
			return matrix;
		}

		/*
		 *enum value
		{
			gen = generate()
		};
		*/
		
	};

private:
	class el_sets
	{
	protected:
		sf::Vector3<float> position_;
		sf::Vector2<float> rotation_;
		el_sets() :
			position_(sf::Vector3<float>(0, 0, 0)),
			rotation_(sf::Vector2<float>(0, 0))
		{
			
		}
		el_sets(const sf::Vector3<float> position, const sf::Vector2<float> rotation) :
			position_(position),
			rotation_(rotation)
		{
			
		}
	public:
		void setPosition(const sf::Vector3<float>& position)
		{
			position_ = position;
		}
		
		void setPosition(const float x, const float y, const float z)
		{
			position_.x = x;
			position_.y = y;
			position_.z = z;
		}

		sf::Vector3<float> getPosition() const
		{
			return position_;
		}
		
		void setRotation(const sf::Vector2<float>& rotation)
		{
			rotation_ = rotation;
		}

		void setRotation(const float x, const float y)
		{
			rotation_.x = x;
			rotation_.y = y;
		}

		sf::Vector2<float> getRotation() const
		{
			return rotation_;
		}

		void rotate(const float x, const float y)
		{
			rotation_.x += x;
			rotation_.y += y;
		}

		void move(const float x, const float y, const float z)
		{
			position_.x += x;
			position_.y += y;
			position_.z += z;
		}
		
	};

public:
	
	class camera
	{
	private:
		void window_init(const sf::Vector2<unsigned>& window_size)
		{
			this->window_size.x = window_size.x;
			this->window_size.y = window_size.y;
			this->window_size_2.x = static_cast<float>(window_size.x) / 2.f;
			this->window_size_2.y = static_cast<float>(window_size.y) / 2.f;
		}
	public:
		//FOV - Field Of View
		sf::Vector2<float> window_size;
		sf::Vector2<float> window_size_2;
		sf::Vector3<float> position;
		sf::Vector2<float> rotation;
		float FOV;
		float zoom;
		float render_constant;


		explicit camera(const sf::RenderWindow& window) :
			window_size(sf::Vector2<int>(window.getSize().x, window.getSize().y)),
			window_size_2(sf::Vector2<int>(window.getSize().x / 2, window.getSize().y / 2)),
			position(sf::Vector3<float>(0, 0, 0)),
			rotation(sf::Vector2<float>(0, 0)),
			FOV(60),
			zoom(1),
			render_constant(static_cast<float>(window_size.x) / abs(2 * (tanf(FOV) * math::PI_180)) * 
				static_cast<float>(window_size.y) * zoom)
		{
			
		}
		
		camera(const sf::RenderWindow& window, const sf::Vector3<float>& position, const float FOV, 
			const sf::Vector2<float>& rotation, const float zoom = 1) :
			window_size(sf::Vector2<int>(window.getSize().x, window.getSize().y)),
			window_size_2(sf::Vector2<int>(window.getSize().x / 2, window.getSize().y / 2)),
			position(position),
			rotation(rotation),
			FOV(FOV),
			zoom(zoom),
			render_constant(static_cast<float>(window_size.x) / abs(2 * (tanf(FOV) * math::PI_180)) * 
				static_cast<float>(window_size.y) * zoom)
		{

		}

		void setWindow(const sf::RenderWindow& window)
		{
			window_init(window.getSize());
			render_constant = static_cast<float>(window_size.x) / abs(2 * (tanf(FOV) * math::PI_180)) *
				static_cast<float>(window_size.y) * zoom;
		}

		void setZoom(const float zoom)
		{
			this->zoom = zoom;
			render_constant = static_cast<float>(window_size.x) / abs(2 * (tanf(FOV) * math::PI_180)) *
				static_cast<float>(window_size.y) * zoom;
		}
		
		void rotate(const float x, const float y)
		{
			rotation.x += x;
			rotation.y += y;
			math::delimit_angle(rotation.x);
			math::delimit_angle(rotation.y);
		}

		void move(const float x, const float y, const float z)
		{
			position.x += x;
			position.y += y;
			position.z += z;
		}
		
		void FOV_unary(const float FOV)
		{
			this->FOV += FOV;
			render_constant = static_cast<float>(window_size.x) / abs(2 * (tanf(FOV) * math::PI_180)) *
				static_cast<float>(window_size.y) * zoom;
		}

		void zoom_unary(const float zoom)
		{
			this->zoom += zoom;
			render_constant = static_cast<float>(window_size.x) / abs(2 * (tanf(FOV) * math::PI_180)) *
				static_cast<float>(window_size.y) * zoom;
		}
	};
	
	//Для этого надо использовать CUDA, ещё в разработке
	class rect : public el_sets
	{
	private:
		sf::Vector2<float> size_;
		sf::Sprite3d Sprite3d_;
		sf::ConvexShape polygon_;
		quad<float> distances_ = quad<float>();
		quad<sf::Vector3<float>> positions_ = quad<sf::Vector3<float>>();

		void init()
		{
			polygon_.setPointCount(4);
			polygon_.setOutlineColor(sf::Color::Red);
			polygon_.setOutlineThickness(3);
		}

	public:
		rect() :
		el_sets(),
		size_(sf::Vector2<float>())
		{
			init();
		}
		rect(const sf::Vector3<float>& position, const sf::Vector2<float> size, const sf::Vector2<float>& rotation) :
		el_sets(position, rotation),
		size_(size)
		{
			init();
		}
		void setSize (const sf::Vector2<float> size)
		{
			size_ = size;
		}
		
		void setSize(const float x, const float y)
		{
			size_.x = x;
			size_.y = y;
		}


		void draw(sf::RenderWindow& window, const sf::Vector3<float>& position_camera, const float& FOV_camera, const sf::Vector2<float>& rotation_camera)
		{
			
			///
			///  a___________b
			///   |			|
			///   |			|
			///	  |			|
			///  c|_________|d
			///

			//Нахождение позиции углов полилигона в двумерном пространстве 
			positions_.a.x = position_.x;
			positions_.a.y = position_.y;
			
			positions_.b.x = position_.x + math::cosf180(rotation_.x) * size_.x;
			positions_.b.y = position_.y + math::sinf180(rotation_.x) * size_.x;

			//std::cout << positions_.c.y << std::endl;
			
			distances_.a = hypot(position_.x - position_camera.x, position_.y - position_camera.y);
			distances_.b = hypot(position_.x - position_camera.x, position_.y - position_camera.y);
			
			/*polygon_.setPoint(0, sf::Vector2f(0, 0));
			polygon_.setPoint(1, sf::Vector2f(0, 10));
			polygon_.setPoint(2, sf::Vector2f(25, 5));
			polygon_.setPosition(10, 20);*/
			window.draw(polygon_);
		}
		
	};
	
	class polygon
	{
	private:
		sf::Sprite3d Sprite3d_;
		sf::ConvexShape polygon_;
		quad<float> distances_ = quad<float>();
		quad<float> angles_ = quad<float>();
		quad<float> signed_angles_ = quad<float>();
		quad<sf::Vector2<float>> screen_positions_ = quad<sf::Vector2<float>>();

		void init()
		{
			polygon_.setPointCount(4);
			polygon_.setOutlineColor(sf::Color::Red);
			polygon_.setOutlineThickness(3);
		}

	public:
		polygon()
		{
			init();
		}

		explicit polygon(const quad<sf::Vector3<float>> positions) :
			positions(positions)
		{
			init();
		}

		quad<sf::Vector3<float>> positions = quad<sf::Vector3<float>>();
		
		void draw(sf::RenderWindow& window, const class camera& camera)
		{

			///
			///  a___________b
			///   |			|
			///   |			|
			///	  |			|
			///  c|_________|d
			///

			//Нахождение позиции углов полилигона в двумерном пространстве 

			distances_.a = hypot(positions.a.x - camera.position.x, positions.a.y - camera.position.y);
			distances_.b = hypot(positions.b.x - camera.position.x, positions.b.y - camera.position.y);
			distances_.c = hypot(positions.c.x - camera.position.x, positions.c.y - camera.position.y);
			distances_.d = hypot(positions.d.x - camera.position.x, positions.d.y - camera.position.y);

			//Нахожнеие глобального угла между камерой и углом полигона
			angles_.a = acosf((positions.a.x - camera.position.x) / distances_.a) *
				(camera.position.y < positions.a.x ? 1 : -1);
			angles_.b = acosf((positions.b.x - camera.position.x) / distances_.b) *
				(camera.position.y < positions.b.x ? 1 : -1);
			angles_.c = acosf((positions.c.x - camera.position.x) / distances_.c) *
				(camera.position.y < positions.c.x ? 1 : -1);
			angles_.d = acosf((positions.d.x - camera.position.x) / distances_.d) *
				(camera.position.y < positions.d.x ? 1 : -1);

			//разница мужду углом камеры и углом полигона
			signed_angles_.a = math::getSignedAngle(camera.rotation.x, math::toDeg(angles_.a));
			signed_angles_.b = math::getSignedAngle(camera.rotation.x, math::toDeg(angles_.b));
			signed_angles_.c = math::getSignedAngle(camera.rotation.x, math::toDeg(angles_.c));
			signed_angles_.d = math::getSignedAngle(camera.rotation.x, math::toDeg(angles_.d));

			//Позиция угла полигона на экране по координате Х
			screen_positions_.a.y = camera.window_size_2.y + camera.render_constant / distances_.a / 2; //+ positions.a.z;
			screen_positions_.b.y = camera.window_size_2.y + camera.render_constant / distances_.b / 2; //+ positions.b.z;
			screen_positions_.c.y = camera.window_size_2.y + camera.render_constant / distances_.c / 2; //+ positions.c.z;
			screen_positions_.d.y = camera.window_size_2.y + camera.render_constant / distances_.d / 2; //+ positions.d.z;

			//Позиция угла полигона на экране по координате У
			screen_positions_.a.x = camera.window_size_2.x + (signed_angles_.a * (camera.window_size.x / camera.FOV));
			screen_positions_.b.x = camera.window_size_2.x + (signed_angles_.b * (camera.window_size.x / camera.FOV));
			screen_positions_.c.x = camera.window_size_2.x + (signed_angles_.c * (camera.window_size.x / camera.FOV));
			screen_positions_.d.x = camera.window_size_2.x + (signed_angles_.d * (camera.window_size.x / camera.FOV));

			//Отображение полигона с помощью SFML
			polygon_.setPoint(0, sf::Vector2<float>(screen_positions_.a.x, screen_positions_.a.y - positions.a.z *
				(camera.render_constant / distances_.a * 0.002)));
			polygon_.setPoint(1, sf::Vector2<float>(screen_positions_.b.x, screen_positions_.b.y - positions.b.z *
				(camera.render_constant / distances_.b * 0.002)));
			polygon_.setPoint(3, sf::Vector2<float>(screen_positions_.c.x, screen_positions_.c.y - positions.c.z *
				(camera.render_constant / distances_.c * 0.002)));
			polygon_.setPoint(2, sf::Vector2<float>(screen_positions_.d.x, screen_positions_.d.y - positions.d.z *
				(camera.render_constant / distances_.d * 0.002)));
			window.draw(polygon_);
		}
	};


public:

	/*class scene
	{
	private:
		std::vector<rect> rects_;
	public:
		class camera camera;
		const sf::Vector2<int> size;
		
		explicit scene(const sf::Vector2<int> size, class camera& camera) :
			camera(camera),
			size(size)
		{
			
		}

		void addElement(const class rect& rect)
		{
			rects_.push_back(rect);
		}
		void operator << (const class rect& rect)
		{
			addElement(rect);
			std::cout << "draw" << std::endl;
		}

		void draw(sf::RenderWindow& window)
		{
			for (unsigned i = 0; i < rects_.size(); i++)
			{
				rects_[i].draw(window, camera.position, camera.FOV, camera.rotation);
			}
			for (unsigned i = 0; i < rects_.size(); i++)
			{
				rects_[i].draw(window, camera.position, camera.FOV, camera.rotation);
			}
		}
		void operator >> (sf::RenderWindow& window)
		{
			draw(window);
		}
	};*/
};
	
