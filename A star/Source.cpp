#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <string>
#include <vector>
#include <set>
#include <queue>
#include <immintrin.h>
using namespace std;

enum Color
{
	BLACK = 0,
	BLUE = 1,
	GREEN = 2,
	CYAN = 3,
	RED = 4,
	MAGENTA = 5,
	BROWN = 6,
	LIGHTGRAY = 7,
	DARKGRAY = 8,
	LIGHTBLUE = 9,
	LIGHTGREEN = 10,
	LIGHTCYAN = 11,
	LIGHTRED = 12,
	LIGHTMAGENTA = 13,
	YELLOW = 14,
	WHITE = 15,

};

void SetColor(WORD color)
{
	HANDLE hConsoleOutput;
	hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_SCREEN_BUFFER_INFO screen_buffer_info;
	GetConsoleScreenBufferInfo(hConsoleOutput, &screen_buffer_info);

	WORD wAttributes = screen_buffer_info.wAttributes;
	color &= 0x000f;
	wAttributes &= 0xfff0;
	wAttributes |= color;

	SetConsoleTextAttribute(hConsoleOutput, wAttributes);
}

void ShowCursor(bool Visible)
{
	CONSOLE_CURSOR_INFO* c = new CONSOLE_CURSOR_INFO;
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

	if (Visible)
	{
		c->bVisible = TRUE;
		c->dwSize = 20;
	}
	else
	{
		c->bVisible = FALSE;
		c->dwSize = 20;
	}

	SetConsoleCursorInfo(h, c);
}

void gotoxy(short x, short y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(
		GetStdHandle(STD_OUTPUT_HANDLE),
		coord
	);
}

class Node
{

public:
	Node(){}
	~Node(){}
	Node(int g,int h,int f)
	{
		g_cost = g;
		h_cost = h;
		f_cost = f;
	}
	Node(int x,int y)
	{
		this->x = x;
		this->y = y;
	}
	int x, y;
	int g_cost = 0;
	int h_cost = 0;
	int f_cost = 0;
	int value = 0;
	Node* parrent;
	int caculate_f_cost()
	{
		f_cost = g_cost + h_cost;
		return f_cost;
	}

};

class Vector2
{
public:
	int x, y;
	Vector2(){}
	Vector2(int x,int y)
	{
		this->x = x;
		this->y = y;
	}

};


class character
{
	
public:
	Vector2 position;
	character(){}
	vector<Node> path;

	bool is_get_destiny()
	{
		return path.empty();
	}

	void move(bool simulate)
	{
		if (!simulate) {
			gotoxy(position.x, position.y);
			cout << ' ';
		}
		int next_move = path.size() - 1;
		position.x = path[next_move].x;
		position.y = path[next_move].y;
		path.pop_back();
		gotoxy(position.x, position.y);
		SetColor(RED);
		cout << char(219);
		SetColor(LIGHTGRAY);
	}

};

class A_star
{
	const int row = 29;
	const int col = 73;
	Node grid[29][73];

	bool** close_list;
	
	vector<Node> open_list;

	int dir_x_8[8] = { 1,-1,1,-1,1,-1,0,0 };
	int dir_y_8[8] = { -1,-1,1,1,0,0,1,-1 };

	int dir_x_4[4] = { 1,-1,0,0 };
	int dir_y_4[4] = { 0,0,1,-1 };

	bool simuale = false;

	int max_y = 29;
	int max_x = 73;

	Node start_node  ;
	Node target_node ;
	int sleep_time = 10;

	enum dir { STOP = 0, LEFT,RIGHT,UP,DOWN };
	dir direction;
	character player;
	character enemy;
	int day = 1;
public:

	void update()
	{
		while(true)
		{
			Sleep(50);
			if (enemy.position.x == player.position.x && enemy.position.y == player.position.y)
			{
				break;
			}

			if(enemy.is_get_destiny())
			{
				enemy.path.clear();
				enemy.path = get_path();
			}
			else
			{
				if (1) {
					
					enemy.move(simuale);
				}
				if (enemy.position.x == player.position.x && enemy.position.y == player.position.y)
				{
					break;
				}
			}

			if(rand() % 100 ==1)
			{
				day++;
			}

			gotoxy(80, 2); cout << "found path";
			
			handle_input();
			draw_player();
		}
	}

	void draw_player()
	{
		
		switch (direction)
		{
		case LEFT:
			{
			if (grid[player.position.y][player.position.x - 1].value == 0) {
				gotoxy(player.position.x, player.position.y);
				cout << ' ';
				player.position.x--;
				gotoxy(player.position.x, player.position.y);
				cout << 'L';
				break;
			}
			}
		case RIGHT:
			{
			if (grid[player.position.y][player.position.x + 1].value == 0) {
				gotoxy(player.position.x, player.position.y);
				cout << ' ';
				player.position.x++;
				gotoxy(player.position.x, player.position.y);
				cout << 'L';
			}
			break;
			}
		case UP:
			{
			if (grid[player.position.y - 1][player.position.x].value == 0) {
				gotoxy(player.position.x, player.position.y);
				cout << ' ';
				player.position.y--;
				gotoxy(player.position.x, player.position.y);
				cout << 'L';
			}
			break;
			}
		case DOWN:
			{
			if (grid[player.position.y + 1][player.position.x].value == 0) {
				gotoxy(player.position.x, player.position.y);
				cout << ' ';
				player.position.y++;
				gotoxy(player.position.x, player.position.y);
				cout << 'L';
			}
			break;
			}
		}

		if (enemy.position.x == player.position.x && enemy.position.y == player.position.y)
		{
			return;
		}
	}

	void handle_input()
	{
		if(_kbhit())
		{
			switch (_getch())
			{
			case 72:
				{
				direction = UP;
				break;
				}
			case 80:
				{
				direction = DOWN;
				break;
				}
			case 75:
				{
				direction = LEFT;
				break;
				}
			case 77:
				{
				direction = RIGHT;
				break;
				}
			}
		}
	}

	A_star()
	{
		player.position = Vector2(5, 20);
		enemy.position = Vector2(70, 3);

		direction = STOP;
		close_list = new bool* [row];
		for(int i = 0 ; i < row;i++)
		{
			close_list[i] = new bool[col];
			for(int j = 0 ; j < col;j++)
			{
				close_list[i][j] = false;
			}
		}

		int map[30][73] = {
			{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,1,1,1,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,1,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,1,1,1,1,1,1,1},
			{1,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
		};

		for(int i = 0 ; i < max_y;i++)
		{
			for(int j = 0 ; j < max_x;j++)
			{
				grid[i][j].value = map[i][j];
				grid[i][j].x = j;
				grid[i][j].y = i;
			}
		}

	}

	void reset_close_list()
	{
		for(int i = 0 ; i < max_y;i++)
		{
			for(int j = 0 ; j < max_x;j++)
			{
				grid[i][j].f_cost = 0;
				grid[i][j].g_cost = 0;
				grid[i][j].h_cost = 0;
				grid[i][j].parrent = nullptr;
				close_list[i][j] = false;
			}
		}
	}

	Node find_target(int value)
	{
		for(int i = 0 ;i < max_y;i++)
		{
			for(int j = 0 ; j < max_x;j++)
			{
				if(grid[i][j].value == value)
				{
					return grid[i][j];
				}
			}
		}
		return Node(0, 0, 0);
	}

	void draw_map()
	{
		for(int i = 0 ; i < max_y;i++)
		{
			for(int j = 0 ; j < max_x;j++)
			{
				if(grid[i][j].value == 1)
				{
					cout << char(219);
				}
				else if(grid[i][j].value == 0)
				{
					cout << ' ';
				}
				else if(grid[i][j].value == 9)
				{
					cout << 'S';
				}
				else if (grid[i][j].value == 3)
				{
					cout << 'A';
				}

			}
			cout << '\n';
		}
		gotoxy(player.position.x, player.position.y);
		cout << 'L';
		gotoxy(enemy.position.x, enemy.position.y);
		cout << 'C';
	}

	void trace_path(Node node)
	{
		SetColor(Color::LIGHTRED);
		while(node.parrent != nullptr)
		{
			gotoxy(node.parrent->x, node.parrent->y);
			cout << char(219);
			Sleep(sleep_time);
			node = *node.parrent;
		}
		SetColor(Color::LIGHTGRAY);
	}

	vector<Node> get_path()
	{
		Node node = a_star();
		vector<Node> temp;
		temp.push_back(node);
		while (node.parrent != nullptr)
		{
			temp.push_back(*node.parrent);
			node = *node.parrent;
		}
		return temp;
	}

	Node a_star()
	{
		reset_close_list();
		open_list.clear();

		target_node = Node(player.position.x, player.position.y);
		start_node = Node(enemy.position.x, enemy.position.y);

		open_list.push_back(start_node);

		while(!open_list.empty())
		{

			Node current = open_list[0];
			int pos = 0;
			
			for(int i = 1 ; i < open_list.size();i++)
			{
				if(open_list[i].f_cost <= current.f_cost)
				{
					if(open_list[i].h_cost < current.h_cost)
					{
						current = open_list[i];
						pos = i;
					}
				}
			}

			open_list.erase(open_list.begin() + pos);

			close_list[current.y][current.x] = true;

			if (simuale) {
				if (current.value == 0)
				{
					gotoxy(current.x, current.y);
					SetColor(Color::CYAN);
					cout << char(219);
					SetColor(Color::LIGHTGRAY);
					Sleep(sleep_time);
				}
			}
			if(current.x == player.position.x && current.y == player.position.y)
			{
				if (simuale) {
					trace_path(current);
				}
				return current;
			}

			explore_neightbours(current.y, current.x);

		}

		return Node(1, 1);
	}

	void explore_neightbours(int y,int x)
	{
		for(int i = 0 ; i < 8;i++)
		{
			int new_y = y + dir_y_8[i];
			int new_x = x + dir_x_8[i];

			if (new_x < 0 || new_x > max_x - 1) continue;
			if (new_y < 0 || new_y > max_y - 1) continue;
			if (close_list[new_y][new_x]) continue;
			if (grid[new_y][new_x].value == 1) continue;

			int path_to_this_neightbour = grid[y][x].g_cost + get_distance(grid[y][x], grid[new_y][new_x]);
			if(path_to_this_neightbour < grid[new_y][new_x].g_cost || !is_queue_contain(grid[new_y][new_x]))
			{

				grid[new_y][new_x].g_cost = path_to_this_neightbour;
				grid[new_y][new_x].h_cost = get_distance(grid[new_y][new_x], target_node);
				grid[new_y][new_x].caculate_f_cost();

				grid[new_y][new_x].parrent = &grid[y][x];

				if(!is_queue_contain(grid[new_y][new_x]))
				{
					if (simuale) {
						gotoxy(new_x, new_y);
						SetColor(Color::DARKGRAY);
						cout << char(219);
						SetColor(Color::LIGHTGRAY);
					}
					open_list.push_back(grid[new_y][new_x]);
				}

			}

		}
	}

	bool is_queue_contain(Node n)
	{
		for(int i = 0 ; i < open_list.size();i++)
		{
			if(open_list[i].x == n.x && open_list[i].y == n.y)
			{
				open_list[i] = n;
				return true;
			}
		}
		return false;
	}



	int get_distance(Node a,Node b)
	{
		int dst_x = abs(a.x - b.x);
		int dst_y = abs(a.y - b.y);


		if(dst_x > dst_y)
		{
			return 5 * dst_y + 3 * (dst_x - dst_y);
		}
		return 5 * dst_x + 3 * (dst_y - dst_x);

	}
	int get_distance(Vector2 a, Vector2 b)
	{
		int dst_x = abs(a.x - b.x);
		int dst_y = abs(a.y - b.y);


		if (dst_x > dst_y)
		{
			return 5 * dst_y + 3 * (dst_x - dst_y);
		}
		return 5 * dst_x + 3 * (dst_y - dst_x);

	}
};


int main()
{
	ShowCursor(false);
	A_star a;
	a.draw_map();
	a.update();

	gotoxy(80, 6);
	cout << "Game Over";

	cin.get();
}