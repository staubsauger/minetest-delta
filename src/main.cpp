/*
Minetest-c55
Copyright (C) 2010 celeron55, Perttu Ahola <celeron55@gmail.com>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

/*
=============================== NOTES ==============================
NOTE: Things starting with TODO are sometimes only suggestions.

NOTE: VBO cannot be turned on for fast-changing stuff because there
      is an apparanet memory leak in irrlicht when using it (not sure)
	  - It is not a memory leak but some kind of a buffer.

NOTE: iostream.imbue(std::locale("C")) is very slow
NOTE: Global locale is now set at initialization

Random suggeestions:
--------------------

SUGG: Fix address to be ipv6 compatible

NOTE: When a new sector is generated, it may change the ground level
      of it's and it's neighbors border that two blocks that are
	  above and below each other and that are generated before and
	  after the sector heightmap generation (order doesn't matter),
	  can have a small gap between each other at the border.
SUGG: Use same technique for sector heightmaps as what we're
      using for UnlimitedHeightmap? (getting all neighbors
	  when generating)

SUGG: Transfer more blocks in a single packet
SUGG: A blockdata combiner class, to which blocks are added and at
      destruction it sends all the stuff in as few packets as possible.

SUGG: If player is on ground, mainly fetch ground-level blocks
SUGG: Fetch stuff mainly from the viewing direction

SUGG: Expose Connection's seqnums and ACKs to server and client.
      - This enables saving many packets and making a faster connection
	  - This also enables server to check if client has received the
	    most recent block sent, for example.
SUGG: Add a sane bandwidth throttling system to Connection

SUGG: More fine-grained control of client's dumping of blocks from
      memory
	  - ...What does this mean in the first place?

SUGG: A map editing mode (similar to dedicated server mode)

SUGG: Add a time value to the param of footstepped grass and check it
      against a global timer when a block is accessed, to make old
	  steps fade away.

SUGG: Make a copy of close-range environment on client for showing
      on screen, with minimal mutexes to slow down the main loop

SUGG: Make a PACKET_COMBINED which contains many subpackets. Utilize
      it by sending more stuff in a single packet.
	  - Add a packet queue to RemoteClient, from which packets will be
	    combined with object data packets
		- This is not exactly trivial: the object data packets are
		  sometimes very big by themselves

SUGG: Split MapBlockObject serialization to to-client and to-disk
      - This will allow saving ages of rats on disk but not sending
	    them to clients

SUGG: MovingObject::move and Player::move are basically the same.
      combine them.
	  - NOTE: Player::move is more up-to-date.

SUGG: Precalculate lighting translation table at runtime (at startup)
      - This is not doable because it is currently hand-made and not
	    based on some mathematical function.
		- Note: This has been changing lately

SUGG: A version number to blocks, which increments when the block is
      modified (node add/remove, water update, lighting update)
	  - This can then be used to make sure the most recent version of
	    a block has been sent to client

SUGG: Make the amount of blocks sending to client and the total
	  amount of blocks dynamically limited. Transferring blocks is the
	  main network eater of this system, so it is the one that has
	  to be throttled so that RTTs stay low.

SUGG: Meshes of blocks could be split into 6 meshes facing into
      different directions and then only those drawn that need to be

SUGG: Calculate lighting per vertex to get a lighting effect like in
      bartwe's game

Gaming ideas:
-------------

- Aim for something like controlling a single dwarf in Dwarf Fortress

- The player could go faster by a crafting a boat, or riding an animal

- Random NPC traders. what else?

Documentation:
--------------

Build system / running:
-----------------------

Networking and serialization:
-----------------------------

TODO: Get rid of GotSplitPacketException

GUI:
----

TODO: Add gui option to remove map

TODO: Configuration menu, at least for keys

Graphics:
---------

TODO: Optimize day/night mesh updating somehow
      - create copies of all textures for all lighting values and only
	    change texture for material?
	  - Umm... the collecting of the faces is the slow part
	    -> what about just changing the color values of the existing
		   meshbuffers? It should go quite fast.
		   - This is not easy; There'd need to be a buffer somewhere
		     that would contain the night and day lighting values.
			 - Actually if FastFaces would be stored, they could
			   hold both values

FEATURE: Combine MapBlock's face caches to so big pieces that VBO
      gets used
      - That is >500 vertices
	  - This is not easy; all the MapBlocks close to the player would
	    still need to be drawn separately and combining the blocks
		would have to happen in a background thread

TODO: Make fetching sector's blocks more efficient when rendering
      sectors that have very large amounts of blocks (on client)
	  - Is this necessary at all?

TODO: Flowing water animation

* Combine meshes to bigger ones in ClientMap and set them EHM_STATIC

SUGG: Draw cubes in inventory directly with 3D drawing commands, so that
      animating them is easier.

Configuration:
--------------

Client:
-------

TODO: Untie client network operations from framerate
      - Needs some input queues or something

SUGG: Make morning and evening transition more smooth and maybe shorter

SUGG: Don't update all meshes always on single node changes, but
      check which ones should be updated
	  - implement Map::updateNodeMeshes()

TODO: Remove IrrlichtWrapper

Server:
-------

SUGG: Make an option to the server to disable building and digging near
      the starting position

TODO: Copy the text of the last picked sign to inventory in creative
      mode

TODO: Check what goes wrong with caching map to disk (Kray)
      - Nothing?

FIXME: Server sometimes goes into some infinite PeerNotFoundException loop

* Fix the problem with the server constantly saving one or a few
  blocks? List the first saved block, maybe it explains.
  - It is probably caused by oscillating water
* Make a small history check to transformLiquids to detect and log
  continuous oscillations, in such detail that they can be fixed.

TODO: Player health points
	- When player dies, throw items on map

FIXME: If something is removed from craftresult with a right click,
	it is only possible to get one item from it should give 4

Objects:
--------

TODO: Get rid of MapBlockObjects

Map:
----

TODO: Mineral and ground material properties
      - This way mineral ground toughness can be calculated with just
	    some formula, as well as tool strengths

TODO: Flowing water to actually contain flow direction information

FEATURE: Create a system that allows a huge amount of different "map
	     generator modules/filters"

FEATURE: Erosion simulation at map generation time
		- Simulate water flows, which would carve out dirt fast and
		  then turn stone into gravel and sand and relocate it.
		- How about relocating minerals, too? Coal and gold in
		  downstream sand and gravel would be kind of cool
		  - This would need a better way of handling minerals, mainly
		    to have mineral content as a separate field. the first
			parameter field is free for this.
		- Simulate rock falling from cliffs when water has removed
		  enough solid rock from the bottom

Mapgen v2:
* only_from_disk might not work anymore - check and fix it.
* Make the generator to run in background and not blocking block
  placement and transfer
* Possibly add some kind of erosion and other stuff
* Make client to fetch stuff asynchronously
  - Needs method SyncProcessData
* Better water generation (spread it to underwater caverns but don't
  fill dungeons that don't touch big water masses)
* When generating a chunk and the neighboring chunk doesn't have mud
  and stuff yet and the ground is fairly flat, the mud will flow to
  the other chunk making nasty straight walls when the other chunk
  is generated. Fix it.

Misc. stuff:
------------
* Make an "environment metafile" to store at least time of day
* Move digging property stuff from material.{h,cpp} to mapnode.cpp...
  - Or maybe move content_features to material.{h,cpp}?
* Maybe:
  Make a system for pregenerating quick information for mapblocks, so
  that the client can show them as cubes before they are actually sent
  or even generated.
* Optimize VoxelManipulator lighting implementation by using indices
  in place of coordinates?

Making it more portable:
------------------------
* Some MSVC: std::sto* are defined without a namespace and collide
  with the ones in utility.h

======================================================================

*/

/*
	Setting this to 1 enables a special camera mode that forces
	the renderers to think that the camera statically points from
	the starting place to a static direction.

	This allows one to move around with the player and see what
	is actually drawn behind solid things and behind the player.
*/
#define FIELD_OF_VIEW_TEST 0

#ifdef NDEBUG
	#ifdef _WIN32
		#pragma message ("Disabling unit tests")
	#else
		#warning "Disabling unit tests"
	#endif
	// Disable unit tests
	#define ENABLE_TESTS 0
#else
	// Enable unit tests
	#define ENABLE_TESTS 1
#endif

#ifdef _MSC_VER
	#pragma comment(lib, "Irrlicht.lib")
	//#pragma comment(lib, "jthread.lib")
	#pragma comment(lib, "zlibwapi.lib")
	#pragma comment(lib, "Shell32.lib")
	// This would get rid of the console window
	//#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

#include <iostream>
#include <fstream>
#include <jmutexautolock.h>
#include <locale.h>
#include "main.h"
#include "common_irrlicht.h"
#include "debug.h"
#include "map.h"
#include "player.h"
#include "test.h"
//#include "environment.h"
#include "server.h"
#include "client.h"
//#include "serialization.h"
#include "constants.h"
//#include "strfnd.h"
#include "porting.h"
#include "irrlichtwrapper.h"
#include "gettime.h"
#include "porting.h"
#include "guiPauseMenu.h"
#include "guiInventoryMenu.h"
#include "guiTextInputMenu.h"
#include "materials.h"
#include "guiMessageMenu.h"
#include "filesys.h"
#include "config.h"
#include "guiMainMenu.h"
#include "mineral.h"
#include "noise.h"
#include "tile.h"

// TODO: Remove this
IrrlichtWrapper *g_irrlicht = NULL;

// This makes textures
ITextureSource *g_texturesource = NULL;

MapDrawControl draw_control;

/*
	Settings.
	These are loaded from the config file.
*/

Settings g_settings;

extern void set_default_settings();

/*
	Random stuff
*/

IrrlichtDevice *g_device = NULL;
Client *g_client = NULL;

/*const s16 quickinv_size = 40;
const s16 quickinv_padding = 8;
const s16 quickinv_spacing = quickinv_size + quickinv_padding;
const s16 quickinv_outer_padding = 4;
const s16 quickinv_itemcount = 8;*/

const s32 hotbar_itemcount = 8;
const s32 hotbar_imagesize = 36;

/*
	GUI Stuff
*/

gui::IGUIEnvironment* guienv = NULL;
gui::IGUIStaticText *guiroot = NULL;

class MainMenuManager : public IMenuManager
{
public:
	virtual void createdMenu(GUIModalMenu *menu)
	{
		for(core::list<GUIModalMenu*>::Iterator
				i = m_stack.begin();
				i != m_stack.end(); i++)
		{
			assert(*i != menu);
		}

		if(m_stack.size() != 0)
			(*m_stack.getLast())->setVisible(false);
		m_stack.push_back(menu);
	}

	virtual void deletingMenu(GUIModalMenu *menu)
	{
		// Remove all entries if there are duplicates
		bool removed_entry;
		do{
			removed_entry = false;
			for(core::list<GUIModalMenu*>::Iterator
					i = m_stack.begin();
					i != m_stack.end(); i++)
			{
				if(*i == menu)
				{
					m_stack.erase(i);
					removed_entry = true;
					break;
				}
			}
		}while(removed_entry);

		/*core::list<GUIModalMenu*>::Iterator i = m_stack.getLast();
		assert(*i == menu);
		m_stack.erase(i);*/
		
		if(m_stack.size() != 0)
			(*m_stack.getLast())->setVisible(true);
	}

	u32 menuCount()
	{
		return m_stack.size();
	}

	core::list<GUIModalMenu*> m_stack;
};

MainMenuManager g_menumgr;

bool noMenuActive()
{
	return (g_menumgr.menuCount() == 0);
}

bool g_disconnect_requested = false;

class MainGameCallback : public IGameCallback
{
public:
	virtual void exitToOS()
	{
		g_device->closeDevice();
	}

	virtual void disconnect()
	{
		g_disconnect_requested = true;
	}
};

MainGameCallback g_gamecallback;

// Inventory actions from the menu are buffered here before sending
// TODO: Get rid of this
Queue<InventoryAction*> inventory_action_queue;
// This is a copy of the inventory that the client's environment has
Inventory local_inventory;

u16 g_selected_item = 0;

/*bool g_show_map_plot = false;
bool g_refresh_map_plot = false;*/

/*
	Debug streams
*/

// Connection
std::ostream *dout_con_ptr = &dummyout;
std::ostream *derr_con_ptr = &dstream_no_stderr;
//std::ostream *dout_con_ptr = &dstream_no_stderr;
//std::ostream *derr_con_ptr = &dstream_no_stderr;
//std::ostream *dout_con_ptr = &dstream;
//std::ostream *derr_con_ptr = &dstream;

// Server
std::ostream *dout_server_ptr = &dstream;
std::ostream *derr_server_ptr = &dstream;

// Client
std::ostream *dout_client_ptr = &dstream;
std::ostream *derr_client_ptr = &dstream;

/*
	gettime.h implementation
*/

u32 getTimeMs()
{
	/*
		Use irrlicht because it is more precise than porting.h's
		getTimeMs()
	*/
	if(g_irrlicht == NULL)
		return 0;
	return g_irrlicht->getTime();
}

/*
	Text input system
*/

struct TextDestSign : public TextDest
{
	TextDestSign(v3s16 blockpos, s16 id, Client *client)
	{
		m_blockpos = blockpos;
		m_id = id;
		m_client = client;
	}
	void gotText(std::wstring text)
	{
		std::string ntext = wide_to_narrow(text);
		dstream<<"Changing text of a sign object: "
				<<ntext<<std::endl;
		m_client->sendSignText(m_blockpos, m_id, ntext);
	}

	v3s16 m_blockpos;
	s16 m_id;
	Client *m_client;
};

struct TextDestChat : public TextDest
{
	TextDestChat(Client *client)
	{
		m_client = client;
	}
	void gotText(std::wstring text)
	{
		// Discard empty line
		if(text == L"")
			return;
		
		// Parse command (server command starts with "/#")
		if(text[0] == L'/' && text[1] != L'#')
		{
			std::wstring reply = L"Local: ";

			reply += L"Local commands not yet supported. "
					L"Server prefix is \"/#\".";
			
			m_client->addChatMessage(reply);
			return;
		}

		// Send to others
		m_client->sendChatMessage(text);
		// Show locally
		m_client->addChatMessage(text);
	}

	Client *m_client;
};

struct TextDestSignNode : public TextDest
{
	TextDestSignNode(v3s16 p, Client *client)
	{
		m_p = p;
		m_client = client;
	}
	void gotText(std::wstring text)
	{
		std::string ntext = wide_to_narrow(text);
		dstream<<"Changing text of a sign node: "
				<<ntext<<std::endl;
		m_client->sendSignNodeText(m_p, ntext);
	}

	v3s16 m_p;
	Client *m_client;
};

class MyEventReceiver : public IEventReceiver
{
public:
	// This is the one method that we have to implement
	virtual bool OnEvent(const SEvent& event)
	{
		/*
			React to nothing here if a menu is active
		*/
		if(noMenuActive() == false)
		{
			clearInput();
			return false;
		}

		// Remember whether each key is down or up
		if(event.EventType == irr::EET_KEY_INPUT_EVENT)
		{
			keyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;

			if(event.KeyInput.PressedDown)
			{
				//dstream<<"Pressed key: "<<(char)event.KeyInput.Key<<std::endl;
				/*if(g_show_map_plot)
				{
					if(event.KeyInput.Key == irr::KEY_ESCAPE
						|| event.KeyInput.Key == irr::KEY_KEY_M)
					{
						g_show_map_plot = false;
					}
					return true;
				}*/
				
				/*
					Launch menus
				*/

				if(guienv != NULL && guiroot != NULL && g_device != NULL)
				{
					if(event.KeyInput.Key == irr::KEY_ESCAPE)
					{
						dstream<<DTIME<<"MyEventReceiver: "
								<<"Launching pause menu"<<std::endl;
						// It will delete itself by itself
						(new GUIPauseMenu(guienv, guiroot, -1, &g_gamecallback,
								&g_menumgr))->drop();
						return true;
					}
					if(event.KeyInput.Key == irr::KEY_KEY_I)
					{
						dstream<<DTIME<<"MyEventReceiver: "
								<<"Launching inventory"<<std::endl;
						
						core::array<GUIInventoryMenu::DrawSpec> draw_spec;
						draw_spec.push_back(GUIInventoryMenu::DrawSpec(
								"list", "current_player", "main",
								v2s32(0, 3), v2s32(8, 4)));
						draw_spec.push_back(GUIInventoryMenu::DrawSpec(
								"list", "current_player", "craft",
								v2s32(3, 0), v2s32(3, 3)));
						draw_spec.push_back(GUIInventoryMenu::DrawSpec(
								"list", "current_player", "craftresult",
								v2s32(7, 1), v2s32(1, 1)));

						GUIInventoryMenu *menu =
							new GUIInventoryMenu(guienv, guiroot, -1,
								&g_menumgr, v2s16(8,7), draw_spec,
								g_client->getInventoryContext(),
								g_client);

						menu->drop();

						return true;
					}
					if(event.KeyInput.Key == irr::KEY_KEY_T)
					{
						TextDest *dest = new TextDestChat(g_client);

						(new GUITextInputMenu(guienv, guiroot, -1,
								&g_menumgr, dest,
								L""))->drop();
					}
				}

				// Item selection
				if(event.KeyInput.Key >= irr::KEY_KEY_0
						&& event.KeyInput.Key <= irr::KEY_KEY_9)
				{
					u16 s1 = event.KeyInput.Key - irr::KEY_KEY_0;
					if(event.KeyInput.Key == irr::KEY_KEY_0)
						s1 = 10;
					if(s1 < PLAYER_INVENTORY_SIZE && s1 < hotbar_itemcount)
						g_selected_item = s1-1;
					dstream<<DTIME<<"Selected item: "
							<<g_selected_item<<std::endl;
				}

				// Viewing range selection
				if(event.KeyInput.Key == irr::KEY_KEY_R)
				{
					if(draw_control.range_all)
					{
						draw_control.range_all = false;
						dstream<<DTIME<<"Disabled full viewing range"<<std::endl;
					}
					else
					{
						draw_control.range_all = true;
						dstream<<DTIME<<"Enabled full viewing range"<<std::endl;
					}
				}

				// Print debug stacks
				if(event.KeyInput.Key == irr::KEY_KEY_P)
				{
					dstream<<"-----------------------------------------"
							<<std::endl;
					dstream<<DTIME<<"Printing debug stacks:"<<std::endl;
					dstream<<"-----------------------------------------"
							<<std::endl;
					debug_stacks_print();
				}

				// Map plot
				/*if(event.KeyInput.Key == irr::KEY_KEY_M)
				{
					dstream<<"Map plot requested"<<std::endl;
					g_show_map_plot = !g_show_map_plot;
					if(g_show_map_plot)
						g_refresh_map_plot = true;
				}*/
				
			}
		}

		if(event.EventType == irr::EET_MOUSE_INPUT_EVENT)
		{
			if(noMenuActive() == false)
			{
				left_active = false;
				middle_active = false;
				right_active = false;
			}
			else
			{
				//dstream<<"MyEventReceiver: mouse input"<<std::endl;
				left_active = event.MouseInput.isLeftPressed();
				middle_active = event.MouseInput.isMiddlePressed();
				right_active = event.MouseInput.isRightPressed();

				if(event.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN)
				{
					leftclicked = true;
				}
				if(event.MouseInput.Event == EMIE_RMOUSE_PRESSED_DOWN)
				{
					rightclicked = true;
				}
				if(event.MouseInput.Event == EMIE_LMOUSE_LEFT_UP)
				{
					leftreleased = true;
				}
				if(event.MouseInput.Event == EMIE_RMOUSE_LEFT_UP)
				{
					rightreleased = true;
				}
				if(event.MouseInput.Event == EMIE_MOUSE_WHEEL)
				{
					/*dstream<<"event.MouseInput.Wheel="
							<<event.MouseInput.Wheel<<std::endl;*/
					
					u16 max_item = MYMIN(PLAYER_INVENTORY_SIZE-1,
							hotbar_itemcount-1);
					if(event.MouseInput.Wheel < 0)
					{
						if(g_selected_item < max_item)
							g_selected_item++;
						else
							g_selected_item = 0;
					}
					else if(event.MouseInput.Wheel > 0)
					{
						if(g_selected_item > 0)
							g_selected_item--;
						else
							g_selected_item = max_item;
					}
				}
			}
		}

		return false;
	}

	// This is used to check whether a key is being held down
	virtual bool IsKeyDown(EKEY_CODE keyCode) const
	{
		return keyIsDown[keyCode];
	}

	void clearInput()
	{
		for (u32 i=0; i<KEY_KEY_CODES_COUNT; ++i)
				keyIsDown[i] = false;
		
		leftclicked = false;
		rightclicked = false;
		leftreleased = false;
		rightreleased = false;

		left_active = false;
		middle_active = false;
		right_active = false;
	}

	MyEventReceiver()
	{
		clearInput();
	}

	bool leftclicked;
	bool rightclicked;
	bool leftreleased;
	bool rightreleased;

	bool left_active;
	bool middle_active;
	bool right_active;

private:
	// We use this array to store the current state of each key
	bool keyIsDown[KEY_KEY_CODES_COUNT];
	//s32 mouseX;
	//s32 mouseY;
	IrrlichtDevice *m_device;
};

class InputHandler
{
public:
	InputHandler()
	{
	}
	virtual ~InputHandler()
	{
	}

	virtual bool isKeyDown(EKEY_CODE keyCode) = 0;

	virtual v2s32 getMousePos() = 0;
	virtual void setMousePos(s32 x, s32 y) = 0;

	virtual bool getLeftState() = 0;
	virtual bool getRightState() = 0;

	virtual bool getLeftClicked() = 0;
	virtual bool getRightClicked() = 0;
	virtual void resetLeftClicked() = 0;
	virtual void resetRightClicked() = 0;

	virtual bool getLeftReleased() = 0;
	virtual bool getRightReleased() = 0;
	virtual void resetLeftReleased() = 0;
	virtual void resetRightReleased() = 0;
	
	virtual void step(float dtime) {};

	virtual void clear() {};
};

InputHandler *g_input = NULL;

class RealInputHandler : public InputHandler
{
public:
	RealInputHandler(IrrlichtDevice *device, MyEventReceiver *receiver):
		m_device(device),
		m_receiver(receiver)
	{
	}
	virtual bool isKeyDown(EKEY_CODE keyCode)
	{
		return m_receiver->IsKeyDown(keyCode);
	}
	virtual v2s32 getMousePos()
	{
		return m_device->getCursorControl()->getPosition();
	}
	virtual void setMousePos(s32 x, s32 y)
	{
		m_device->getCursorControl()->setPosition(x, y);
	}

	virtual bool getLeftState()
	{
		return m_receiver->left_active;
	}
	virtual bool getRightState()
	{
		return m_receiver->right_active;
	}
	
	virtual bool getLeftClicked()
	{
		return m_receiver->leftclicked;
	}
	virtual bool getRightClicked()
	{
		return m_receiver->rightclicked;
	}
	virtual void resetLeftClicked()
	{
		m_receiver->leftclicked = false;
	}
	virtual void resetRightClicked()
	{
		m_receiver->rightclicked = false;
	}

	virtual bool getLeftReleased()
	{
		return m_receiver->leftreleased;
	}
	virtual bool getRightReleased()
	{
		return m_receiver->rightreleased;
	}
	virtual void resetLeftReleased()
	{
		m_receiver->leftreleased = false;
	}
	virtual void resetRightReleased()
	{
		m_receiver->rightreleased = false;
	}

	void clear()
	{
		resetRightClicked();
		resetLeftClicked();
	}
private:
	IrrlichtDevice *m_device;
	MyEventReceiver *m_receiver;
};

class RandomInputHandler : public InputHandler
{
public:
	RandomInputHandler()
	{
		leftdown = false;
		rightdown = false;
		leftclicked = false;
		rightclicked = false;
		leftreleased = false;
		rightreleased = false;
		for(u32 i=0; i<KEY_KEY_CODES_COUNT; ++i)
			keydown[i] = false;
	}
	virtual bool isKeyDown(EKEY_CODE keyCode)
	{
		return keydown[keyCode];
	}
	virtual v2s32 getMousePos()
	{
		return mousepos;
	}
	virtual void setMousePos(s32 x, s32 y)
	{
		mousepos = v2s32(x,y);
	}

	virtual bool getLeftState()
	{
		return leftdown;
	}
	virtual bool getRightState()
	{
		return rightdown;
	}

	virtual bool getLeftClicked()
	{
		return leftclicked;
	}
	virtual bool getRightClicked()
	{
		return rightclicked;
	}
	virtual void resetLeftClicked()
	{
		leftclicked = false;
	}
	virtual void resetRightClicked()
	{
		rightclicked = false;
	}

	virtual bool getLeftReleased()
	{
		return leftreleased;
	}
	virtual bool getRightReleased()
	{
		return rightreleased;
	}
	virtual void resetLeftReleased()
	{
		leftreleased = false;
	}
	virtual void resetRightReleased()
	{
		rightreleased = false;
	}

	virtual void step(float dtime)
	{
		{
			static float counter1 = 0;
			counter1 -= dtime;
			if(counter1 < 0.0)
			{
				counter1 = 0.1*Rand(1, 40);
				keydown[irr::KEY_SPACE] = !keydown[irr::KEY_SPACE];
			}
		}
		{
			static float counter1 = 0;
			counter1 -= dtime;
			if(counter1 < 0.0)
			{
				counter1 = 0.1*Rand(1, 40);
				keydown[irr::KEY_KEY_E] = !keydown[irr::KEY_KEY_E];
			}
		}
		{
			static float counter1 = 0;
			counter1 -= dtime;
			if(counter1 < 0.0)
			{
				counter1 = 0.1*Rand(1, 40);
				keydown[irr::KEY_KEY_W] = !keydown[irr::KEY_KEY_W];
			}
		}
		{
			static float counter1 = 0;
			counter1 -= dtime;
			if(counter1 < 0.0)
			{
				counter1 = 0.1*Rand(1, 40);
				keydown[irr::KEY_KEY_A] = !keydown[irr::KEY_KEY_A];
			}
		}
		{
			static float counter1 = 0;
			counter1 -= dtime;
			if(counter1 < 0.0)
			{
				counter1 = 0.1*Rand(1, 20);
				mousespeed = v2s32(Rand(-20,20), Rand(-15,20));
			}
		}
		{
			static float counter1 = 0;
			counter1 -= dtime;
			if(counter1 < 0.0)
			{
				counter1 = 0.1*Rand(1, 30);
				leftdown = !leftdown;
				if(leftdown)
					leftclicked = true;
				if(!leftdown)
					leftreleased = true;
			}
		}
		{
			static float counter1 = 0;
			counter1 -= dtime;
			if(counter1 < 0.0)
			{
				counter1 = 0.1*Rand(1, 15);
				rightdown = !rightdown;
				if(rightdown)
					rightclicked = true;
				if(!rightdown)
					rightreleased = true;
			}
		}
		mousepos += mousespeed;
	}

	s32 Rand(s32 min, s32 max)
	{
		return (myrand()%(max-min+1))+min;
	}
private:
	bool keydown[KEY_KEY_CODES_COUNT];
	v2s32 mousepos;
	v2s32 mousespeed;
	bool leftdown;
	bool rightdown;
	bool leftclicked;
	bool rightclicked;
	bool leftreleased;
	bool rightreleased;
};

void updateViewingRange(f32 frametime_in, Client *client)
{
	if(draw_control.range_all == true)
		return;
	
	static f32 added_frametime = 0;
	static s16 added_frames = 0;

	added_frametime += frametime_in;
	added_frames += 1;

	// Actually this counter kind of sucks because frametime is busytime
	static f32 counter = 0;
	counter -= frametime_in;
	if(counter > 0)
		return;
	//counter = 0.1;
	counter = 0.2;

	/*dstream<<__FUNCTION_NAME
			<<": Collected "<<added_frames<<" frames, total of "
			<<added_frametime<<"s."<<std::endl;*/
	
	/*dstream<<"draw_control.blocks_drawn="
			<<draw_control.blocks_drawn
			<<", draw_control.blocks_would_have_drawn="
			<<draw_control.blocks_would_have_drawn
			<<std::endl;*/
	
	float range_min = g_settings.getS16("viewing_range_nodes_min");
	float range_max = g_settings.getS16("viewing_range_nodes_max");
	
	draw_control.wanted_min_range = range_min;
	draw_control.wanted_max_blocks = (1.2*draw_control.blocks_drawn)+1;
	
	float block_draw_ratio = 1.0;
	if(draw_control.blocks_would_have_drawn != 0)
	{
		block_draw_ratio = (float)draw_control.blocks_drawn
			/ (float)draw_control.blocks_would_have_drawn;
	}

	// Calculate the average frametime in the case that all wanted
	// blocks had been drawn
	f32 frametime = added_frametime / added_frames / block_draw_ratio;
	
	added_frametime = 0.0;
	added_frames = 0;
	
	float wanted_fps = g_settings.getFloat("wanted_fps");
	float wanted_frametime = 1.0 / wanted_fps;
	
	f32 wanted_frametime_change = wanted_frametime - frametime;
	//dstream<<"wanted_frametime_change="<<wanted_frametime_change<<std::endl;
	
	// If needed frametime change is small, just return
	if(fabs(wanted_frametime_change) < wanted_frametime*0.4)
	{
		//dstream<<"ignoring small wanted_frametime_change"<<std::endl;
		return;
	}

	float range = draw_control.wanted_range;
	float new_range = range;

	static s16 range_old = 0;
	static f32 frametime_old = 0;
	
	float d_range = range - range_old;
	f32 d_frametime = frametime - frametime_old;
	// A sane default of 30ms per 50 nodes of range
	static f32 time_per_range = 30. / 50;
	if(d_range != 0)
	{
		time_per_range = d_frametime / d_range;
	}
	
	// The minimum allowed calculated frametime-range derivative:
	// Practically this sets the maximum speed of changing the range.
	// The lower this value, the higher the maximum changing speed.
	// A low value here results in wobbly range (0.001)
	// A high value here results in slow changing range (0.0025)
	// SUGG: This could be dynamically adjusted so that when
	//       the camera is turning, this is lower
	//float min_time_per_range = 0.0015;
	float min_time_per_range = 0.0010;
	//float min_time_per_range = 0.05 / range;
	if(time_per_range < min_time_per_range)
	{
		time_per_range = min_time_per_range;
		//dstream<<"time_per_range="<<time_per_range<<" (min)"<<std::endl;
	}
	else
	{
		//dstream<<"time_per_range="<<time_per_range<<std::endl;
	}

	f32 wanted_range_change = wanted_frametime_change / time_per_range;
	// Dampen the change a bit to kill oscillations
	//wanted_range_change *= 0.9;
	//wanted_range_change *= 0.75;
	wanted_range_change *= 0.5;
	//dstream<<"wanted_range_change="<<wanted_range_change<<std::endl;

	// If needed range change is very small, just return
	if(fabs(wanted_range_change) < 0.001)
	{
		//dstream<<"ignoring small wanted_range_change"<<std::endl;
		return;
	}

	new_range += wanted_range_change;
	//dstream<<"new_range="<<new_range/*<<std::endl*/;
	
	//float new_range_unclamped = new_range;
	if(new_range < range_min)
		new_range = range_min;
	if(new_range > range_max)
		new_range = range_max;
	
	/*if(new_range != new_range_unclamped)
		dstream<<", clamped to "<<new_range<<std::endl;
	else
		dstream<<std::endl;*/

	draw_control.wanted_range = new_range;

	range_old = new_range;
	frametime_old = frametime;
}

void draw_hotbar(video::IVideoDriver *driver, gui::IGUIFont *font,
		v2s32 centerlowerpos, s32 imgsize, s32 itemcount,
		Inventory *inventory)
{
	InventoryList *mainlist = inventory->getList("main");
	if(mainlist == NULL)
	{
		dstream<<"WARNING: draw_hotbar(): mainlist == NULL"<<std::endl;
		return;
	}
	
	s32 padding = imgsize/12;
	//s32 height = imgsize + padding*2;
	s32 width = itemcount*(imgsize+padding*2);
	
	// Position of upper left corner of bar
	v2s32 pos = centerlowerpos - v2s32(width/2, imgsize+padding*2);
	
	// Draw background color
	/*core::rect<s32> barrect(0,0,width,height);
	barrect += pos;
	video::SColor bgcolor(255,128,128,128);
	driver->draw2DRectangle(bgcolor, barrect, NULL);*/

	core::rect<s32> imgrect(0,0,imgsize,imgsize);

	for(s32 i=0; i<itemcount; i++)
	{
		InventoryItem *item = mainlist->getItem(i);
		
		core::rect<s32> rect = imgrect + pos
				+ v2s32(padding+i*(imgsize+padding*2), padding);
		
		if(g_selected_item == i)
		{
			driver->draw2DRectangle(video::SColor(255,255,0,0),
					core::rect<s32>(rect.UpperLeftCorner - v2s32(1,1)*padding,
							rect.LowerRightCorner + v2s32(1,1)*padding),
					NULL);
		}
		else
		{
			video::SColor bgcolor2(128,0,0,0);
			driver->draw2DRectangle(bgcolor2, rect, NULL);
		}

		if(item != NULL)
		{
			drawInventoryItem(driver, font, item, rect, NULL);
		}
	}
}

#if 0
video::ITexture *g_map_plot_texture = NULL;
float g_map_plot_texture_scale = 4;

void updateMapPlotTexture(v2f centerpos, video::IVideoDriver* driver,
		Client *client)
{
	assert(driver);
	assert(client);

	core::dimension2d<u32> dim(640,480);
	video::IImage *img = driver->createImage(video::ECF_A8R8G8B8, dim);
	assert(img);
	for(u32 y=0; y<dim.Height; y++)
	for(u32 x=0; x<dim.Width; x++)
	{
		v2f pf = v2f(x, dim.Height-y) - v2f(dim.Width, dim.Height)/2;
		pf *= g_map_plot_texture_scale;
		pf += centerpos;
		double h = base_rock_level_2d(client->getMapSeed(), pf);
		video::SColor c;
		//double d1 = 50;
		/*s32 ux = x - centerpos.X / g_map_plot_texture_scale;
		s32 uy = y - centerpos.Y / g_map_plot_texture_scale;*/
			
		// Screen coordinates that are based on multiples of
		// 1000/g_map_plot_texture_scale and never negative
		u32 ux = x + (u32)(1000/g_map_plot_texture_scale) * 10;
		u32 uy = y + (u32)(1000/g_map_plot_texture_scale) * 10;
		// Offset to center of image
		ux -= dim.Width/2;
		uy -= dim.Height/2;

		if(uy % (u32)(1000/g_map_plot_texture_scale) == 0
				|| ux % (u32)(1000/g_map_plot_texture_scale) == 0)
			c.set(255, 255, 255, 255);
		else if(uy % (u32)(100/g_map_plot_texture_scale) == 0
				|| ux % (u32)(100/g_map_plot_texture_scale) == 0)
			c.set(255, 160, 160, 160);
		else if(h < WATER_LEVEL - 0.5) // Water
			c.set(255, 50, 50, 255);
#if 0
		else if(get_have_sand_ground(client->getMapSeed(), pf)
				|| (h < WATER_LEVEL + 2
				&& get_have_sand_coast(client->getMapSeed(), pf)))
		{
			h -= WATER_LEVEL;
			h /= 50.0;
			h = 1.0 - exp(-h);

			video::SColor c1(255,237,201,175);
			//video::SColor c2(255,20,20,20);
			video::SColor c2(255,150,0,0);
			c = c2.getInterpolated(c1, h);
		}
		else
		{
			h -= WATER_LEVEL;
			h /= 50.0;
			h = 1.0 - exp(-h);

			video::SColor c1(255,110,185,90);
			//video::SColor c2(255,20,20,20);
			video::SColor c2(255,150,0,0);
			c = c2.getInterpolated(c1, h);
		}
#endif
#if 1
#if 0
		else if(get_have_sand_ground(client->getMapSeed(), pf))
		{
			h -= WATER_LEVEL;
			h /= 20.0;
			h = 1.0 - exp(-h);

			video::SColor c1(255,237,201,175);
			//video::SColor c2(255,20,20,20);
			video::SColor c2(255,150,0,0);
			c = c2.getInterpolated(c1, h);
		}
#endif
		// Sand
		else if(h < WATER_LEVEL + 2
				&& get_have_sand_coast(client->getMapSeed(), pf))
			c.set(255, 237, 201, 175);
#if 1
		else if(h < WATER_LEVEL + 10)
			c.set(255, 50, 150, 50); // Green
		else if(h < WATER_LEVEL + 20)
			c.set(255, 110, 185, 50); // Yellowish green
		else if(h < WATER_LEVEL + 40)
			c.set(255, 180, 210, 50); // Greenish yellow
		else if(h < WATER_LEVEL + 60)
			c.set(255, 220, 220, 50); // Yellow
		else if(h < WATER_LEVEL + 80)
			c.set(255, 200, 200, 110); // Yellowish white
		else if(h < WATER_LEVEL + 100)
			c.set(255, 190, 190, 190); // Grey
		else
			c.set(255, 255, 255, 255); // White
#endif
#endif
#if 0
		else
		{
			h -= WATER_LEVEL;
			h /= 20.0;
			h = 1.0 - exp(-h);

			video::SColor c1(255,200,200,50);
			video::SColor c2(255,0,150,0);
			c = c1.getInterpolated(c2, h);

			/*u32 a = (u32)(h*255);
			if(a > 255)
				a = 255;
			a = 255-a;
			c.set(255, a, a, a);*/
		}
#endif
#if 1
		if(h >= WATER_LEVEL - 0.5
			&& get_have_sand_ground(client->getMapSeed(), pf))
		{
			video::SColor c1(255,237,201,175);
			c = c.getInterpolated(c1, 0.5);
		}
#endif
#if 1
		double tf = get_turbulence_factor_2d(client->getMapSeed(), pf);
		if(tf > 0.001)
		{
			video::SColor c1(255,255,0,0);
			c = c.getInterpolated(c1, 1.0-(0.5*tf));
		}
#endif
		img->setPixel(x, y, c);
	}
	g_map_plot_texture = driver->addTexture("map_plot", img);
	img->drop();
	assert(g_map_plot_texture);
}
#endif

// Chat data
struct ChatLine
{
	ChatLine():
		age(0.0)
	{
	}
	ChatLine(const std::wstring &a_text):
		age(0.0),
		text(a_text)
	{
	}
	float age;
	std::wstring text;
};

// These are defined global so that they're not optimized too much.
// Can't change them to volatile.
s16 temp16;
f32 tempf;
v3f tempv3f1;
v3f tempv3f2;
std::string tempstring;
std::string tempstring2;

void SpeedTests()
{
	{
		dstream<<"The following test should take around 20ms."<<std::endl;
		TimeTaker timer("Testing std::string speed");
		const u32 jj = 10000;
		for(u32 j=0; j<jj; j++)
		{
			tempstring = "";
			tempstring2 = "";
			const u32 ii = 10;
			for(u32 i=0; i<ii; i++){
				tempstring2 += "asd";
			}
			for(u32 i=0; i<ii+1; i++){
				tempstring += "asd";
				if(tempstring == tempstring2)
					break;
			}
		}
	}
	
	dstream<<"All of the following tests should take around 100ms each."
			<<std::endl;

	{
		TimeTaker timer("Testing floating-point conversion speed");
		tempf = 0.001;
		for(u32 i=0; i<4000000; i++){
			temp16 += tempf;
			tempf += 0.001;
		}
	}
	
	{
		TimeTaker timer("Testing floating-point vector speed");

		tempv3f1 = v3f(1,2,3);
		tempv3f2 = v3f(4,5,6);
		for(u32 i=0; i<10000000; i++){
			tempf += tempv3f1.dotProduct(tempv3f2);
			tempv3f2 += v3f(7,8,9);
		}
	}

	{
		TimeTaker timer("Testing core::map speed");
		
		core::map<v2s16, f32> map1;
		tempf = -324;
		const s16 ii=300;
		for(s16 y=0; y<ii; y++){
			for(s16 x=0; x<ii; x++){
				map1.insert(v2s16(x,y), tempf);
				tempf += 1;
			}
		}
		for(s16 y=ii-1; y>=0; y--){
			for(s16 x=0; x<ii; x++){
				tempf = map1[v2s16(x,y)];
			}
		}
	}

	{
		dstream<<"Around 5000/ms should do well here."<<std::endl;
		TimeTaker timer("Testing mutex speed");
		
		JMutex m;
		m.Init();
		u32 n = 0;
		u32 i = 0;
		do{
			n += 10000;
			for(; i<n; i++){
				m.Lock();
				m.Unlock();
			}
		}
		// Do at least 10ms
		while(timer.getTime() < 10);

		u32 dtime = timer.stop();
		u32 per_ms = n / dtime;
		std::cout<<"Done. "<<dtime<<"ms, "
				<<per_ms<<"/ms"<<std::endl;
	}
}

int main(int argc, char *argv[])
{
	/*
		Parse command line
	*/
	
	// List all allowed options
	core::map<std::string, ValueSpec> allowed_options;
	allowed_options.insert("help", ValueSpec(VALUETYPE_FLAG));
	allowed_options.insert("server", ValueSpec(VALUETYPE_FLAG,
			"Run server directly"));
	allowed_options.insert("config", ValueSpec(VALUETYPE_STRING,
			"Load configuration from specified file"));
	allowed_options.insert("port", ValueSpec(VALUETYPE_STRING));
	allowed_options.insert("address", ValueSpec(VALUETYPE_STRING));
	allowed_options.insert("random-input", ValueSpec(VALUETYPE_FLAG));
	allowed_options.insert("disable-unittests", ValueSpec(VALUETYPE_FLAG));
	allowed_options.insert("enable-unittests", ValueSpec(VALUETYPE_FLAG));
	allowed_options.insert("map-dir", ValueSpec(VALUETYPE_STRING));
#ifdef _WIN32
	allowed_options.insert("dstream-on-stderr", ValueSpec(VALUETYPE_FLAG));
#endif
	allowed_options.insert("speedtests", ValueSpec(VALUETYPE_FLAG));

	Settings cmd_args;
	
	bool ret = cmd_args.parseCommandLine(argc, argv, allowed_options);

	if(ret == false || cmd_args.getFlag("help"))
	{
		dstream<<"Allowed options:"<<std::endl;
		for(core::map<std::string, ValueSpec>::Iterator
				i = allowed_options.getIterator();
				i.atEnd() == false; i++)
		{
			dstream<<"  --"<<i.getNode()->getKey();
			if(i.getNode()->getValue().type == VALUETYPE_FLAG)
			{
			}
			else
			{
				dstream<<" <value>";
			}
			dstream<<std::endl;

			if(i.getNode()->getValue().help != NULL)
			{
				dstream<<"      "<<i.getNode()->getValue().help
						<<std::endl;
			}
		}

		return cmd_args.getFlag("help") ? 0 : 1;
	}
	
	/*
		Low-level initialization
	*/

	bool disable_stderr = false;
#ifdef _WIN32
	if(cmd_args.getFlag("dstream-on-stderr") == false)
		disable_stderr = true;
#endif

	// Initialize debug streams
	debugstreams_init(disable_stderr, DEBUGFILE);
	// Initialize debug stacks
	debug_stacks_init();

	DSTACK(__FUNCTION_NAME);

	porting::signal_handler_init();
	bool &kill = *porting::signal_handler_killstatus();
	
	porting::initializePaths();
	// Create user data directory
	fs::CreateDir(porting::path_userdata);
	
	// C-style stuff initialization
	initializeMaterialProperties();

	// Debug handler
	BEGIN_DEBUG_EXCEPTION_HANDLER

	// Print startup message
	dstream<<DTIME<<"minetest-c55"
			" with SER_FMT_VER_HIGHEST="<<(int)SER_FMT_VER_HIGHEST
			<<", "<<BUILD_INFO
			<<std::endl;
	
	/*
		Basic initialization
	*/

	// Initialize default settings
	set_default_settings();
	
	// Set locale. This is for forcing '.' as the decimal point.
	std::locale::global(std::locale("C"));
	// This enables printing all characters in bitmap font
	setlocale(LC_CTYPE, "en_US");

	// Initialize sockets
	sockets_init();
	atexit(sockets_cleanup);
	
	/*
		Initialization
	*/

	/*
		Read config file
	*/
	
	// Path of configuration file in use
	std::string configpath = "";
	
	if(cmd_args.exists("config"))
	{
		bool r = g_settings.readConfigFile(cmd_args.get("config").c_str());
		if(r == false)
		{
			dstream<<"Could not read configuration from \""
					<<cmd_args.get("config")<<"\""<<std::endl;
			return 1;
		}
		configpath = cmd_args.get("config");
	}
	else
	{
		core::array<std::string> filenames;
		filenames.push_back(porting::path_userdata + "/minetest.conf");
#ifdef RUN_IN_PLACE
		filenames.push_back(porting::path_userdata + "/../minetest.conf");
#endif

		for(u32 i=0; i<filenames.size(); i++)
		{
			bool r = g_settings.readConfigFile(filenames[i].c_str());
			if(r)
			{
				configpath = filenames[i];
				break;
			}
		}
		
		// If no path found, use the first one (menu creates the file)
		if(configpath == "")
			configpath = filenames[0];
	}

	// Initialize random seed
	srand(time(0));
	mysrand(time(0));

	/*
		Pre-initialize some stuff with a dummy irrlicht wrapper.

		These are needed for unit tests at least.
	*/
	
	// Initial call with g_texturesource not set.
	init_mapnode();

	/*
		Run unit tests
	*/

	if((ENABLE_TESTS && cmd_args.getFlag("disable-unittests") == false)
			|| cmd_args.getFlag("enable-unittests") == true)
	{
		run_tests();
	}
	
	/*for(s16 y=-100; y<100; y++)
	for(s16 x=-100; x<100; x++)
	{
		std::cout<<noise2d_gradient((double)x/10,(double)y/10, 32415)<<std::endl;
	}
	return 0;*/
	
	/*
		Some parameters
	*/

	// Port
	u16 port = 30000;
	if(cmd_args.exists("port"))
		port = cmd_args.getU16("port");
	else if(cmd_args.exists("port"))
		port = g_settings.getU16("port");
	
	// Map directory
	std::string map_dir = porting::path_userdata+"/map";
	if(cmd_args.exists("map-dir"))
		map_dir = cmd_args.get("map-dir");
	else if(g_settings.exists("map-dir"))
		map_dir = g_settings.get("map-dir");
	
	// Run dedicated server if asked to
	if(cmd_args.getFlag("server"))
	{
		DSTACK("Dedicated server branch");

		// Create server
		Server server(map_dir.c_str());
		server.start(port);
		
		// Run server
		dedicated_server_loop(server, kill);

		return 0;
	}

	/*
		More parameters
	*/
	
	// Address to connect to
	std::string address = "";
	
	if(cmd_args.exists("address"))
	{
		address = cmd_args.get("address");
	}
	else
	{
		address = g_settings.get("address");
	}
	
	std::string playername = g_settings.get("name");

	// Resolution selection
	
	bool fullscreen = false;
	u16 screenW = g_settings.getU16("screenW");
	u16 screenH = g_settings.getU16("screenH");

	// Determine driver

	video::E_DRIVER_TYPE driverType;
	
	std::string driverstring = g_settings.get("video_driver");

	if(driverstring == "null")
		driverType = video::EDT_NULL;
	else if(driverstring == "software")
		driverType = video::EDT_SOFTWARE;
	else if(driverstring == "burningsvideo")
		driverType = video::EDT_BURNINGSVIDEO;
	else if(driverstring == "direct3d8")
		driverType = video::EDT_DIRECT3D8;
	else if(driverstring == "direct3d9")
		driverType = video::EDT_DIRECT3D9;
	else if(driverstring == "opengl")
		driverType = video::EDT_OPENGL;
	else
	{
		dstream<<"WARNING: Invalid video_driver specified; defaulting "
				"to opengl"<<std::endl;
		driverType = video::EDT_OPENGL;
	}

	// create device and exit if creation failed

	MyEventReceiver receiver;

	IrrlichtDevice *device;
	device = createDevice(driverType,
			core::dimension2d<u32>(screenW, screenH),
			16, fullscreen, false, false, &receiver);

	if (device == 0)
		return 1; // could not create selected driver.
	
	g_device = device;
	g_irrlicht = new IrrlichtWrapper(device);
	TextureSource *texturesource = new TextureSource(device);
	g_texturesource = texturesource;

	/*
		Speed tests (done after irrlicht is loaded to get timer)
	*/
	if(cmd_args.getFlag("speedtests"))
	{
		dstream<<"Running speed tests"<<std::endl;
		SpeedTests();
		return 0;
	}
	
	device->setResizable(true);

	bool random_input = g_settings.getBool("random_input")
			|| cmd_args.getFlag("random-input");
	if(random_input)
		g_input = new RandomInputHandler();
	else
		g_input = new RealInputHandler(device, &receiver);
	
	/*
		Continue initialization
	*/

	video::IVideoDriver* driver = device->getVideoDriver();

	/*
		This changes the minimum allowed number of vertices in a VBO.
		Default is 500.
	*/
	//driver->setMinHardwareBufferVertexCount(50);

	scene::ISceneManager* smgr = device->getSceneManager();

	guienv = device->getGUIEnvironment();
	gui::IGUISkin* skin = guienv->getSkin();
	gui::IGUIFont* font = guienv->getFont(porting::getDataPath("fontlucida.png").c_str());
	if(font)
		skin->setFont(font);
	else
		dstream<<"WARNING: Font file was not found."
				" Using default font."<<std::endl;
	// If font was not found, this will get us one
	font = skin->getFont();
	assert(font);

	u32 text_height = font->getDimension(L"Hello, world!").Height;
	dstream<<"text_height="<<text_height<<std::endl;

	//skin->setColor(gui::EGDC_BUTTON_TEXT, video::SColor(255,0,0,0));
	skin->setColor(gui::EGDC_BUTTON_TEXT, video::SColor(255,255,255,255));
	//skin->setColor(gui::EGDC_3D_HIGH_LIGHT, video::SColor(0,0,0,0));
	//skin->setColor(gui::EGDC_3D_SHADOW, video::SColor(0,0,0,0));
	skin->setColor(gui::EGDC_3D_HIGH_LIGHT, video::SColor(255,0,0,0));
	skin->setColor(gui::EGDC_3D_SHADOW, video::SColor(255,0,0,0));
	
	/*
		Preload some textures and stuff
	*/

	init_content_inventory_texture_paths();
	init_mapnode(); // Second call with g_texturesource set
	init_mineral();

	/*
		GUI stuff
	*/

	/*
		We need some kind of a root node to be able to add
		custom gui elements directly on the screen.
		Otherwise they won't be automatically drawn.
	*/
	guiroot = guienv->addStaticText(L"",
			core::rect<s32>(0, 0, 10000, 10000));
	
	// First line of debug text
	gui::IGUIStaticText *guitext = guienv->addStaticText(
			L"",
			core::rect<s32>(5, 5, 795, 5+text_height),
			false, false);
	// Second line of debug text
	gui::IGUIStaticText *guitext2 = guienv->addStaticText(
			L"",
			core::rect<s32>(5, 5+(text_height+5)*1, 795, (5+text_height)*2),
			false, false);
	
	// At the middle of the screen
	// Object infos are shown in this
	gui::IGUIStaticText *guitext_info = guienv->addStaticText(
			L"",
			core::rect<s32>(0,0,400,text_height+5) + v2s32(100,200),
			false, false);
	
	// Chat text
	gui::IGUIStaticText *guitext_chat = guienv->addStaticText(
			L"",
			core::rect<s32>(0,0,0,0),
			false, false); // Disable word wrap as of now
			//false, true);
	//guitext_chat->setBackgroundColor(video::SColor(96,0,0,0));
	core::list<ChatLine> chat_lines;
	
	/*
		If an error occurs, this is set to something and the
		menu-game loop is restarted. It is then displayed before
		the menu.
	*/
	std::wstring error_message = L"";
	
	/*
		Menu-game loop
	*/
	while(g_device->run() && kill == false)
	{
	
	// This is used for catching disconnects
	try
	{
	
	/*
		Out-of-game menu loop.

		Loop quits when menu returns proper parameters.
	*/
	while(kill == false)
	{
		// Cursor can be non-visible when coming from the game
		device->getCursorControl()->setVisible(true);
		// Some stuff are left to scene manager when coming from the game
		// (map at least?)
		smgr->clear();
		// Reset or hide the debug gui texts
		guitext->setText(L"Minetest-c55");
		guitext2->setVisible(false);
		guitext_info->setVisible(false);
		guitext_chat->setVisible(false);
		
		// Initialize menu data
		MainMenuData menudata;
		menudata.address = narrow_to_wide(address);
		menudata.name = narrow_to_wide(playername);
		menudata.port = narrow_to_wide(itos(port));
		menudata.creative_mode = g_settings.getBool("creative_mode");

		GUIMainMenu *menu =
				new GUIMainMenu(guienv, guiroot, -1, 
					&g_menumgr, &menudata, &g_gamecallback);
		menu->allowFocusRemoval(true);

		if(error_message != L"")
		{
			GUIMessageMenu *menu2 =
					new GUIMessageMenu(guienv, guiroot, -1, 
						&g_menumgr, error_message.c_str());
			menu2->drop();
			error_message = L"";
		}

		video::IVideoDriver* driver = g_device->getVideoDriver();
		
		dstream<<"Created main menu"<<std::endl;

		while(g_device->run() && kill == false)
		{
			// Run global IrrlichtWrapper's main thread processing stuff
			g_irrlicht->Run();
			
			if(menu->getStatus() == true)
				break;

			//driver->beginScene(true, true, video::SColor(255,0,0,0));
			driver->beginScene(true, true, video::SColor(255,128,128,128));
			guienv->drawAll();
			driver->endScene();
		}
		
		// Break out of menu-game loop to shut down cleanly
		if(g_device->run() == false || kill == true)
			break;
		
		dstream<<"Dropping main menu"<<std::endl;

		menu->drop();
		
		// Delete map if requested
		if(menudata.delete_map)
		{
			bool r = fs::RecursiveDeleteContent(map_dir);
			if(r == false)
				error_message = L"Delete failed";
			continue;
		}

		playername = wide_to_narrow(menudata.name);
		address = wide_to_narrow(menudata.address);
		port = stoi(wide_to_narrow(menudata.port));
		g_settings.set("creative_mode", itos(menudata.creative_mode));
		
		// Check for valid parameters, restart menu if invalid.
		if(playername == "")
		{
			error_message = L"Name required.";
			continue;
		}
		
		// Save settings
		g_settings.set("name", playername);
		g_settings.set("address", address);
		g_settings.set("port", itos(port));
		// Update configuration file
		if(configpath != "")
			g_settings.updateConfigFile(configpath.c_str());
	
		// Continue to game
		break;
	}
	
	// Break out of menu-game loop to shut down cleanly
	if(g_device->run() == false)
		break;

	/*
		Make a scope here so that the client and the server and other
		stuff gets removed when disconnected or the irrlicht device
		is removed.
	*/
	{

	// This is set to true at the end of the scope
	g_irrlicht->Shutdown(false);

	/*
		Draw "Loading" screen
	*/
	const wchar_t *text = L"Loading and connecting...";
	core::vector2d<s32> center(screenW/2, screenH/2);
	core::vector2d<s32> textsize(300, text_height);
	core::rect<s32> textrect(center - textsize/2, center + textsize/2);

	gui::IGUIStaticText *gui_loadingtext = guienv->addStaticText(
			text, textrect, false, false);
	gui_loadingtext->setTextAlignment(gui::EGUIA_CENTER, gui::EGUIA_UPPERLEFT);

	driver->beginScene(true, true, video::SColor(255,0,0,0));
	guienv->drawAll();
	driver->endScene();

	std::cout<<DTIME<<"Creating server and client"<<std::endl;
	
	/*
		Create server.
		SharedPtr will delete it when it goes out of scope.
	*/
	SharedPtr<Server> server;
	if(address == ""){
		server = new Server(map_dir);
		server->start(port);
	}
	
	/*
		Create client
	*/

	Client client(device, playername.c_str(), draw_control);
			
	g_client = &client;
	
	Address connect_address(0,0,0,0, port);
	try{
		if(address == "")
			//connect_address.Resolve("localhost");
			connect_address.setAddress(127,0,0,1);
		else
			connect_address.Resolve(address.c_str());
	}
	catch(ResolveError &e)
	{
		std::cout<<DTIME<<"Couldn't resolve address"<<std::endl;
		//return 0;
		error_message = L"Couldn't resolve address";
		gui_loadingtext->remove();
		continue;
	}
	
	dstream<<DTIME<<"Connecting to server at ";
	connect_address.print(&dstream);
	dstream<<std::endl;
	client.connect(connect_address);
	
	try{
		while(client.connectedAndInitialized() == false)
		{
			// Update screen
			driver->beginScene(true, true, video::SColor(255,0,0,0));
			guienv->drawAll();
			driver->endScene();

			// Update client and server

			client.step(0.1);

			if(server != NULL)
				server->step(0.1);
			
			// Delay a bit
			sleep_ms(100);
		}
	}
	catch(con::PeerNotFoundException &e)
	{
		std::cout<<DTIME<<"Timed out."<<std::endl;
		//return 0;
		error_message = L"Connection timed out.";
		gui_loadingtext->remove();
		continue;
	}

	/*
		Create skybox
	*/
	/*scene::ISceneNode* skybox;
	skybox = smgr->addSkyBoxSceneNode(
		driver->getTexture(porting::getDataPath("skybox2.png").c_str()),
		driver->getTexture(porting::getDataPath("skybox3.png").c_str()),
		driver->getTexture(porting::getDataPath("skybox1.png").c_str()),
		driver->getTexture(porting::getDataPath("skybox1.png").c_str()),
		driver->getTexture(porting::getDataPath("skybox1.png").c_str()),
		driver->getTexture(porting::getDataPath("skybox1.png").c_str()));*/
	
	/*
		Create the camera node
	*/

	scene::ICameraSceneNode* camera = smgr->addCameraSceneNode(
		0, // Camera parent
		v3f(BS*100, BS*2, BS*100), // Look from
		v3f(BS*100+1, BS*2, BS*100), // Look to
		-1 // Camera ID
   	);

	if(camera == NULL)
		return 1;

	//video::SColor skycolor = video::SColor(255,90,140,200);
	//video::SColor skycolor = video::SColor(255,166,202,244);
	video::SColor skycolor = video::SColor(255,120,185,244);

	camera->setFOV(FOV_ANGLE);

	// Just so big a value that everything rendered is visible
	camera->setFarValue(100000*BS);
	
	/*
		Lighting test code. Doesn't quite work this way.
		The CPU-computed lighting is good.
	*/

	/*
	smgr->addLightSceneNode(NULL,
		v3f(0, BS*1000000, 0),
		video::SColorf(0.3,0.3,0.3),
		BS*10000000);

	smgr->setAmbientLight(video::SColorf(0.0, 0.0, 0.0));

	scene::ILightSceneNode *light = smgr->addLightSceneNode(camera,
			v3f(0, 0, 0), video::SColorf(0.5,0.5,0.5), BS*4);
	*/

	f32 camera_yaw = 0; // "right/left"
	f32 camera_pitch = 0; // "up/down"

	/*
		Move into game
	*/
	
	gui_loadingtext->remove();

	/*
		Add some gui stuff
	*/

	/*GUIQuickInventory *quick_inventory = new GUIQuickInventory
			(guienv, NULL, v2s32(10, 70), 5, &local_inventory);*/
	/*GUIQuickInventory *quick_inventory = new GUIQuickInventory
			(guienv, NULL, v2s32(0, 0), quickinv_itemcount, &local_inventory);*/
	
	// Test the text input system
	/*(new GUITextInputMenu(guienv, guiroot, -1, &g_menumgr,
			NULL))->drop();*/
	/*GUIMessageMenu *menu =
			new GUIMessageMenu(guienv, guiroot, -1, 
				&g_menumgr,
				L"Asd");
	menu->drop();*/
	
	// Launch pause menu
	(new GUIPauseMenu(guienv, guiroot, -1, &g_gamecallback,
			&g_menumgr))->drop();
	
	// Enable texts
	guitext2->setVisible(true);
	guitext_info->setVisible(true);
	guitext_chat->setVisible(true);

	//s32 guitext_chat_pad_bottom = 70;

	v2u32 screensize(0,0);
	v2u32 last_screensize(0,0);
	
	/*
		Some statistics are collected in these
	*/
	u32 drawtime = 0;
	u32 beginscenetime = 0;
	u32 scenetime = 0;
	u32 endscenetime = 0;
	
	// A test
	//throw con::PeerNotFoundException("lol");

	core::list<float> frametime_log;

	/*
		Main loop
	*/

	bool first_loop_after_window_activation = true;

	// Time is in milliseconds
	// NOTE: getRealTime() causes strange problems in wine (imprecision?)
	// NOTE: So we have to use getTime() and call run()s between them
	u32 lasttime = device->getTimer()->getTime();

	while(device->run() && kill == false)
	{
		if(g_disconnect_requested)
		{
			g_disconnect_requested = false;
			break;
		}

		/*
			Run global IrrlichtWrapper's main thread processing stuff
		*/
		g_irrlicht->Run();

		/*
			Process TextureSource's queue
		*/
		texturesource->processQueue();

		/*
			Random calculations
		*/
		last_screensize = screensize;
		screensize = driver->getScreenSize();
		v2s32 displaycenter(screensize.X/2,screensize.Y/2);
		//bool screensize_changed = screensize != last_screensize;
		
		// Hilight boxes collected during the loop and displayed
		core::list< core::aabbox3d<f32> > hilightboxes;
		
		// Info text
		std::wstring infotext;

		// When screen size changes, update positions and sizes of stuff
		/*if(screensize_changed)
		{
			v2s32 pos(displaycenter.X-((quickinv_itemcount-1)*quickinv_spacing+quickinv_size)/2, screensize.Y-quickinv_spacing);
			quick_inventory->updatePosition(pos);
		}*/

		//TimeTaker //timer1("//timer1");
		
		// Time of frame without fps limit
		float busytime;
		u32 busytime_u32;
		{
			// not using getRealTime is necessary for wine
			u32 time = device->getTimer()->getTime();
			if(time > lasttime)
				busytime_u32 = time - lasttime;
			else
				busytime_u32 = 0;
			busytime = busytime_u32 / 1000.0;
		}

		//std::cout<<"busytime_u32="<<busytime_u32<<std::endl;
	
		// Necessary for device->getTimer()->getTime()
		device->run();

		/*
			Viewing range
		*/
		
		updateViewingRange(busytime, &client);
		
		/*
			FPS limiter
		*/

		{
			float fps_max = g_settings.getFloat("fps_max");
			u32 frametime_min = 1000./fps_max;
			
			if(busytime_u32 < frametime_min)
			{
				u32 sleeptime = frametime_min - busytime_u32;
				device->sleep(sleeptime);
			}
		}

		// Necessary for device->getTimer()->getTime()
		device->run();

		/*
			Time difference calculation
		*/
		f32 dtime; // in seconds
		
		u32 time = device->getTimer()->getTime();
		if(time > lasttime)
			dtime = (time - lasttime) / 1000.0;
		else
			dtime = 0;
		lasttime = time;

		/*
			Log frametime for visualization
		*/
		frametime_log.push_back(dtime);
		if(frametime_log.size() > 100)
		{
			core::list<float>::Iterator i = frametime_log.begin();
			frametime_log.erase(i);
		}

		/*
			Visualize frametime in terminal
		*/
		/*for(u32 i=0; i<dtime*400; i++)
			std::cout<<"X";
		std::cout<<std::endl;*/

		/*
			Time average and jitter calculation
		*/

		static f32 dtime_avg1 = 0.0;
		dtime_avg1 = dtime_avg1 * 0.98 + dtime * 0.02;
		f32 dtime_jitter1 = dtime - dtime_avg1;

		static f32 dtime_jitter1_max_sample = 0.0;
		static f32 dtime_jitter1_max_fraction = 0.0;
		{
			static f32 jitter1_max = 0.0;
			static f32 counter = 0.0;
			if(dtime_jitter1 > jitter1_max)
				jitter1_max = dtime_jitter1;
			counter += dtime;
			if(counter > 0.0)
			{
				counter -= 3.0;
				dtime_jitter1_max_sample = jitter1_max;
				dtime_jitter1_max_fraction
						= dtime_jitter1_max_sample / (dtime_avg1+0.001);
				jitter1_max = 0.0;
			}
		}
		
		/*
			Busytime average and jitter calculation
		*/

		static f32 busytime_avg1 = 0.0;
		busytime_avg1 = busytime_avg1 * 0.98 + busytime * 0.02;
		f32 busytime_jitter1 = busytime - busytime_avg1;
		
		static f32 busytime_jitter1_max_sample = 0.0;
		static f32 busytime_jitter1_min_sample = 0.0;
		{
			static f32 jitter1_max = 0.0;
			static f32 jitter1_min = 0.0;
			static f32 counter = 0.0;
			if(busytime_jitter1 > jitter1_max)
				jitter1_max = busytime_jitter1;
			if(busytime_jitter1 < jitter1_min)
				jitter1_min = busytime_jitter1;
			counter += dtime;
			if(counter > 0.0){
				counter -= 3.0;
				busytime_jitter1_max_sample = jitter1_max;
				busytime_jitter1_min_sample = jitter1_min;
				jitter1_max = 0.0;
				jitter1_min = 0.0;
			}
		}
		
		/*
			Debug info for client
		*/
		{
			static float counter = 0.0;
			counter -= dtime;
			if(counter < 0)
			{
				counter = 30.0;
				client.printDebugInfo(std::cout);
			}
		}

		/*
			Input handler step()
		*/
		g_input->step(dtime);

		/*
			Misc. stuff
		*/

		/*
			Player speed control
		*/
		
		{
			/*bool a_up,
			bool a_down,
			bool a_left,
			bool a_right,
			bool a_jump,
			bool a_superspeed,
			bool a_sneak,
			float a_pitch,
			float a_yaw*/
			PlayerControl control(
				g_input->isKeyDown(irr::KEY_KEY_W),
				g_input->isKeyDown(irr::KEY_KEY_S),
				g_input->isKeyDown(irr::KEY_KEY_A),
				g_input->isKeyDown(irr::KEY_KEY_D),
				g_input->isKeyDown(irr::KEY_SPACE),
				g_input->isKeyDown(irr::KEY_KEY_E),
				g_input->isKeyDown(irr::KEY_LSHIFT)
						|| g_input->isKeyDown(irr::KEY_RSHIFT),
				camera_pitch,
				camera_yaw
			);
			client.setPlayerControl(control);
		}

		/*
			Process environment
		*/
		
		{
			//TimeTaker timer("client.step(dtime)");
			client.step(dtime);
			//client.step(dtime_avg1);
		}

		if(server != NULL)
		{
			//TimeTaker timer("server->step(dtime)");
			server->step(dtime);
		}

		v3f player_position = client.getPlayerPosition();
		
		//TimeTaker //timer2("//timer2");

		/*
			Mouse and camera control
		*/
		
		if((device->isWindowActive() && noMenuActive()) || random_input)
		{
			if(!random_input)
				device->getCursorControl()->setVisible(false);

			if(first_loop_after_window_activation){
				//std::cout<<"window active, first loop"<<std::endl;
				first_loop_after_window_activation = false;
			}
			else{
				s32 dx = g_input->getMousePos().X - displaycenter.X;
				s32 dy = g_input->getMousePos().Y - displaycenter.Y;
				//std::cout<<"window active, pos difference "<<dx<<","<<dy<<std::endl;
				camera_yaw -= dx*0.2;
				camera_pitch += dy*0.2;
				if(camera_pitch < -89.5) camera_pitch = -89.5;
				if(camera_pitch > 89.5) camera_pitch = 89.5;
			}
			g_input->setMousePos(displaycenter.X, displaycenter.Y);
		}
		else{
			device->getCursorControl()->setVisible(true);

			//std::cout<<"window inactive"<<std::endl;
			first_loop_after_window_activation = true;
		}

		camera_yaw = wrapDegrees(camera_yaw);
		camera_pitch = wrapDegrees(camera_pitch);
		
		v3f camera_direction = v3f(0,0,1);
		camera_direction.rotateYZBy(camera_pitch);
		camera_direction.rotateXZBy(camera_yaw);
		
		// This is at the height of the eyes of the current figure
		//v3f camera_position = player_position + v3f(0, BS+BS/2, 0);
		// This is more like in minecraft
		v3f camera_position = player_position + v3f(0, BS+BS*0.625, 0);

		camera->setPosition(camera_position);
		// *100.0 helps in large map coordinates
		camera->setTarget(camera_position + camera_direction * 100.0);

		if(FIELD_OF_VIEW_TEST){
			client.updateCamera(v3f(0,0,0), v3f(0,0,1));
		}
		else{
			//TimeTaker timer("client.updateCamera");
			client.updateCamera(camera_position, camera_direction);
		}
		
		//timer2.stop();
		//TimeTaker //timer3("//timer3");

		/*
			Calculate what block is the crosshair pointing to
		*/
		
		//u32 t1 = device->getTimer()->getRealTime();
		
		//f32 d = 4; // max. distance
		f32 d = 4; // max. distance
		core::line3d<f32> shootline(camera_position,
				camera_position + camera_direction * BS * (d+1));

		MapBlockObject *selected_object = client.getSelectedObject
				(d*BS, camera_position, shootline);

		/*
			If it's pointing to a MapBlockObject
		*/

		if(selected_object != NULL)
		{
			//dstream<<"Client returned selected_object != NULL"<<std::endl;

			core::aabbox3d<f32> box_on_map
					= selected_object->getSelectionBoxOnMap();

			hilightboxes.push_back(box_on_map);

			infotext = narrow_to_wide(selected_object->infoText());

			if(g_input->getLeftClicked())
			{
				std::cout<<DTIME<<"Left-clicked object"<<std::endl;
				client.clickObject(0, selected_object->getBlock()->getPos(),
						selected_object->getId(), g_selected_item);
			}
			else if(g_input->getRightClicked())
			{
				std::cout<<DTIME<<"Right-clicked object"<<std::endl;
				/*
					Check if we want to modify the object ourselves
				*/
				if(selected_object->getTypeId() == MAPBLOCKOBJECT_TYPE_SIGN)
				{
					dstream<<"Sign object right-clicked"<<std::endl;
					
					if(random_input == false)
					{
						// Get a new text for it

						TextDest *dest = new TextDestSign(
								selected_object->getBlock()->getPos(),
								selected_object->getId(),
								&client);

						SignObject *sign_object = (SignObject*)selected_object;

						std::wstring wtext =
								narrow_to_wide(sign_object->getText());

						(new GUITextInputMenu(guienv, guiroot, -1,
								&g_menumgr, dest,
								wtext))->drop();
					}
				}
				/*
					Otherwise pass the event to the server as-is
				*/
				else
				{
					client.clickObject(1, selected_object->getBlock()->getPos(),
							selected_object->getId(), g_selected_item);
				}
			}
		}
		else // selected_object == NULL
		{

		/*
			Find out which node we are pointing at
		*/
		
		bool nodefound = false;
		v3s16 nodepos;
		v3s16 neighbourpos;
		core::aabbox3d<f32> nodehilightbox;
		f32 mindistance = BS * 1001;
		
		v3s16 pos_i = floatToInt(player_position, BS);

		/*std::cout<<"pos_i=("<<pos_i.X<<","<<pos_i.Y<<","<<pos_i.Z<<")"
				<<std::endl;*/

		s16 a = d;
		s16 ystart = pos_i.Y + 0 - (camera_direction.Y<0 ? a : 1);
		s16 zstart = pos_i.Z - (camera_direction.Z<0 ? a : 1);
		s16 xstart = pos_i.X - (camera_direction.X<0 ? a : 1);
		s16 yend = pos_i.Y + 1 + (camera_direction.Y>0 ? a : 1);
		s16 zend = pos_i.Z + (camera_direction.Z>0 ? a : 1);
		s16 xend = pos_i.X + (camera_direction.X>0 ? a : 1);
		
		for(s16 y = ystart; y <= yend; y++)
		for(s16 z = zstart; z <= zend; z++)
		for(s16 x = xstart; x <= xend; x++)
		{
			MapNode n;
			try
			{
				n = client.getNode(v3s16(x,y,z));
				if(content_pointable(n.d) == false)
					continue;
			}
			catch(InvalidPositionException &e)
			{
				continue;
			}

			v3s16 np(x,y,z);
			v3f npf = intToFloat(np, BS);
			
			f32 d = 0.01;
			
			v3s16 dirs[6] = {
				v3s16(0,0,1), // back
				v3s16(0,1,0), // top
				v3s16(1,0,0), // right
				v3s16(0,0,-1), // front
				v3s16(0,-1,0), // bottom
				v3s16(-1,0,0), // left
			};
			
			/*
				Meta-objects
			*/
			if(n.d == CONTENT_TORCH)
			{
				v3s16 dir = unpackDir(n.dir);
				v3f dir_f = v3f(dir.X, dir.Y, dir.Z);
				dir_f *= BS/2 - BS/6 - BS/20;
				v3f cpf = npf + dir_f;
				f32 distance = (cpf - camera_position).getLength();

				core::aabbox3d<f32> box;
				
				// bottom
				if(dir == v3s16(0,-1,0))
				{
					box = core::aabbox3d<f32>(
						npf - v3f(BS/6, BS/2, BS/6),
						npf + v3f(BS/6, -BS/2+BS/3*2, BS/6)
					);
				}
				// top
				else if(dir == v3s16(0,1,0))
				{
					box = core::aabbox3d<f32>(
						npf - v3f(BS/6, -BS/2+BS/3*2, BS/6),
						npf + v3f(BS/6, BS/2, BS/6)
					);
				}
				// side
				else
				{
					box = core::aabbox3d<f32>(
						cpf - v3f(BS/6, BS/3, BS/6),
						cpf + v3f(BS/6, BS/3, BS/6)
					);
				}

				if(distance < mindistance)
				{
					if(box.intersectsWithLine(shootline))
					{
						nodefound = true;
						nodepos = np;
						neighbourpos = np;
						mindistance = distance;
						nodehilightbox = box;
					}
				}
			}
			else if(n.d == CONTENT_SIGN_WALL)
			{
				v3s16 dir = unpackDir(n.dir);
				v3f dir_f = v3f(dir.X, dir.Y, dir.Z);
				dir_f *= BS/2 - BS/6 - BS/20;
				v3f cpf = npf + dir_f;
				f32 distance = (cpf - camera_position).getLength();

				v3f vertices[4] =
				{
					v3f(BS*0.42,-BS*0.35,-BS*0.4),
					v3f(BS*0.49, BS*0.35, BS*0.4),
				};

				for(s32 i=0; i<2; i++)
				{
					if(dir == v3s16(1,0,0))
						vertices[i].rotateXZBy(0);
					if(dir == v3s16(-1,0,0))
						vertices[i].rotateXZBy(180);
					if(dir == v3s16(0,0,1))
						vertices[i].rotateXZBy(90);
					if(dir == v3s16(0,0,-1))
						vertices[i].rotateXZBy(-90);
					if(dir == v3s16(0,-1,0))
						vertices[i].rotateXYBy(-90);
					if(dir == v3s16(0,1,0))
						vertices[i].rotateXYBy(90);

					vertices[i] += npf;
				}

				core::aabbox3d<f32> box;

				box = core::aabbox3d<f32>(vertices[0]);
				box.addInternalPoint(vertices[1]);

				if(distance < mindistance)
				{
					if(box.intersectsWithLine(shootline))
					{
						nodefound = true;
						nodepos = np;
						neighbourpos = np;
						mindistance = distance;
						nodehilightbox = box;
					}
				}
			}
			/*
				Regular blocks
			*/
			else
			{
				for(u16 i=0; i<6; i++)
				{
					v3f dir_f = v3f(dirs[i].X,
							dirs[i].Y, dirs[i].Z);
					v3f centerpoint = npf + dir_f * BS/2;
					f32 distance =
							(centerpoint - camera_position).getLength();
					
					if(distance < mindistance)
					{
						core::CMatrix4<f32> m;
						m.buildRotateFromTo(v3f(0,0,1), dir_f);

						// This is the back face
						v3f corners[2] = {
							v3f(BS/2, BS/2, BS/2),
							v3f(-BS/2, -BS/2, BS/2+d)
						};
						
						for(u16 j=0; j<2; j++)
						{
							m.rotateVect(corners[j]);
							corners[j] += npf;
						}

						core::aabbox3d<f32> facebox(corners[0]);
						facebox.addInternalPoint(corners[1]);

						if(facebox.intersectsWithLine(shootline))
						{
							nodefound = true;
							nodepos = np;
							neighbourpos = np + dirs[i];
							mindistance = distance;

							//nodehilightbox = facebox;

							const float d = 0.502;
							core::aabbox3d<f32> nodebox
									(-BS*d, -BS*d, -BS*d, BS*d, BS*d, BS*d);
							v3f nodepos_f = intToFloat(nodepos, BS);
							nodebox.MinEdge += nodepos_f;
							nodebox.MaxEdge += nodepos_f;
							nodehilightbox = nodebox;
						}
					} // if distance < mindistance
				} // for dirs
			} // regular block
		} // for coords

		static float nodig_delay_counter = 0.0;

		if(nodefound)
		{
			static v3s16 nodepos_old(-32768,-32768,-32768);

			static float dig_time = 0.0;
			static u16 dig_index = 0;
			
			/*
				Visualize selection
			*/

			hilightboxes.push_back(nodehilightbox);

			/*
				Check information text of node
			*/

			NodeMetadata *meta = client.getNodeMetadata(nodepos);
			if(meta)
			{
				infotext = narrow_to_wide(meta->infoText());
			}
			
			//MapNode node = client.getNode(nodepos);

			/*
				Handle digging
			*/
			
			if(g_input->getLeftReleased())
			{
				client.clearTempMod(nodepos);
				dig_time = 0.0;
			}
			
			if(nodig_delay_counter > 0.0)
			{
				nodig_delay_counter -= dtime;
			}
			else
			{
				if(nodepos != nodepos_old)
				{
					std::cout<<DTIME<<"Pointing at ("<<nodepos.X<<","
							<<nodepos.Y<<","<<nodepos.Z<<")"<<std::endl;

					if(nodepos_old != v3s16(-32768,-32768,-32768))
					{
						client.clearTempMod(nodepos_old);
						dig_time = 0.0;
					}
				}

				if(g_input->getLeftClicked() ||
						(g_input->getLeftState() && nodepos != nodepos_old))
				{
					dstream<<DTIME<<"Started digging"<<std::endl;
					client.groundAction(0, nodepos, neighbourpos, g_selected_item);
				}
				if(g_input->getLeftClicked())
				{
					client.setTempMod(nodepos, NodeMod(NODEMOD_CRACK, 0));
				}
				if(g_input->getLeftState())
				{
					MapNode n = client.getNode(nodepos);
				
					// Get tool name. Default is "" = bare hands
					std::string toolname = "";
					InventoryList *mlist = local_inventory.getList("main");
					if(mlist != NULL)
					{
						InventoryItem *item = mlist->getItem(g_selected_item);
						if(item && (std::string)item->getName() == "ToolItem")
						{
							ToolItem *titem = (ToolItem*)item;
							toolname = titem->getToolName();
						}
					}

					// Get digging properties for material and tool
					u8 material = n.d;
					DiggingProperties prop =
							getDiggingProperties(material, toolname);
					
					float dig_time_complete = 0.0;

					if(prop.diggable == false)
					{
						/*dstream<<"Material "<<(int)material
								<<" not diggable with \""
								<<toolname<<"\""<<std::endl;*/
						// I guess nobody will wait for this long
						dig_time_complete = 10000000.0;
					}
					else
					{
						dig_time_complete = prop.time;
					}
					
					if(dig_time_complete >= 0.001)
					{
						dig_index = (u16)((float)CRACK_ANIMATION_LENGTH
								* dig_time/dig_time_complete);
					}
					// This is for torches
					else
					{
						dig_index = CRACK_ANIMATION_LENGTH;
					}

					if(dig_index < CRACK_ANIMATION_LENGTH)
					{
						//TimeTaker timer("client.setTempMod");
						//dstream<<"dig_index="<<dig_index<<std::endl;
						client.setTempMod(nodepos, NodeMod(NODEMOD_CRACK, dig_index));
					}
					else
					{
						dstream<<DTIME<<"Digging completed"<<std::endl;
						client.groundAction(3, nodepos, neighbourpos, g_selected_item);
						client.clearTempMod(nodepos);
						client.removeNode(nodepos);

						dig_time = 0;

						nodig_delay_counter = dig_time_complete
								/ (float)CRACK_ANIMATION_LENGTH;

						// We don't want a corresponding delay to
						// very time consuming nodes
						if(nodig_delay_counter > 0.5)
						{
							nodig_delay_counter = 0.5;
						}
						// We want a slight delay to very little
						// time consuming nodes
						float mindelay = 0.15;
						if(nodig_delay_counter < mindelay)
						{
							nodig_delay_counter = mindelay;
						}
					}

					dig_time += dtime;
				}
			}
			
			if(g_input->getRightClicked())
			{
				std::cout<<DTIME<<"Ground right-clicked"<<std::endl;
				
				if(meta && meta->typeId() == CONTENT_SIGN_WALL && !random_input)
				{
					dstream<<"Sign node right-clicked"<<std::endl;
					
					SignNodeMetadata *signmeta = (SignNodeMetadata*)meta;
					
					// Get a new text for it

					TextDest *dest = new TextDestSignNode(nodepos, &client);

					std::wstring wtext =
							narrow_to_wide(signmeta->getText());

					(new GUITextInputMenu(guienv, guiroot, -1,
							&g_menumgr, dest,
							wtext))->drop();
				}
				else if(meta && meta->typeId() == CONTENT_CHEST && !random_input)
				{
					dstream<<"Chest node right-clicked"<<std::endl;
					
					//ChestNodeMetadata *chestmeta = (ChestNodeMetadata*)meta;

					core::array<GUIInventoryMenu::DrawSpec> draw_spec;
					
					std::string chest_inv_id;
					chest_inv_id += "nodemeta:";
					chest_inv_id += itos(nodepos.X);
					chest_inv_id += ",";
					chest_inv_id += itos(nodepos.Y);
					chest_inv_id += ",";
					chest_inv_id += itos(nodepos.Z);
					
					draw_spec.push_back(GUIInventoryMenu::DrawSpec(
							"list", chest_inv_id, "0",
							v2s32(0, 0), v2s32(8, 4)));
					draw_spec.push_back(GUIInventoryMenu::DrawSpec(
							"list", "current_player", "main",
							v2s32(0, 5), v2s32(8, 4)));

					GUIInventoryMenu *menu =
						new GUIInventoryMenu(guienv, guiroot, -1,
							&g_menumgr, v2s16(8,9), draw_spec,
							g_client->getInventoryContext(),
							g_client);

					menu->drop();

				}
				else
				{
					client.groundAction(1, nodepos, neighbourpos, g_selected_item);
				}
			}
			
			nodepos_old = nodepos;
		}
		else{
		}

		} // selected_object == NULL
		
		g_input->resetLeftClicked();
		g_input->resetRightClicked();
		
		if(g_input->getLeftReleased())
		{
			std::cout<<DTIME<<"Left button released (stopped digging)"
					<<std::endl;
			client.groundAction(2, v3s16(0,0,0), v3s16(0,0,0), 0);
		}
		if(g_input->getRightReleased())
		{
			//std::cout<<DTIME<<"Right released"<<std::endl;
			// Nothing here
		}
		
		g_input->resetLeftReleased();
		g_input->resetRightReleased();
		
		/*
			Calculate stuff for drawing
		*/

		camera->setAspectRatio((f32)screensize.X / (f32)screensize.Y);
		
		u32 daynight_ratio = client.getDayNightRatio();
		u8 l = decode_light((daynight_ratio * LIGHT_SUN) / 1000);
		video::SColor bgcolor = video::SColor(
				255,
				skycolor.getRed() * l / 255,
				skycolor.getGreen() * l / 255,
				skycolor.getBlue() * l / 255);

		/*
			Fog
		*/
		
		if(g_settings.getBool("enable_fog") == true)
		{
			//f32 range = draw_control.wanted_range * BS + MAP_BLOCKSIZE/2*BS;
			f32 range = draw_control.wanted_range * BS + 0.8*MAP_BLOCKSIZE*BS;
			//f32 range = draw_control.wanted_range * BS + 0.0*MAP_BLOCKSIZE*BS;
			if(draw_control.range_all)
				range = 100000*BS;

			driver->setFog(
				bgcolor,
				video::EFT_FOG_LINEAR,
				range*0.4,
				range*1.0,
				0.01,
				false, // pixel fog
				false // range fog
			);
		}
		else
		{
			driver->setFog(
				bgcolor,
				video::EFT_FOG_LINEAR,
				100000*BS,
				110000*BS,
				0.01,
				false, // pixel fog
				false // range fog
			);
		}


		/*
			Update gui stuff (0ms)
		*/

		//TimeTaker guiupdatetimer("Gui updating");
		
		{
			static float drawtime_avg = 0;
			drawtime_avg = drawtime_avg * 0.95 + (float)drawtime*0.05;
			static float beginscenetime_avg = 0;
			beginscenetime_avg = beginscenetime_avg * 0.95 + (float)beginscenetime*0.05;
			static float scenetime_avg = 0;
			scenetime_avg = scenetime_avg * 0.95 + (float)scenetime*0.05;
			static float endscenetime_avg = 0;
			endscenetime_avg = endscenetime_avg * 0.95 + (float)endscenetime*0.05;
			
			char temptext[300];
			snprintf(temptext, 300, "Minetest-c55 ("
					"F: item=%i"
					", R: range_all=%i"
					")"
					" drawtime=%.0f, beginscenetime=%.0f"
					", scenetime=%.0f, endscenetime=%.0f",
					g_selected_item,
					draw_control.range_all,
					drawtime_avg,
					beginscenetime_avg,
					scenetime_avg,
					endscenetime_avg
					);
			
			guitext->setText(narrow_to_wide(temptext).c_str());
		}
		
		{
			char temptext[300];
			snprintf(temptext, 300,
					"(% .1f, % .1f, % .1f)"
					" (% .3f < btime_jitter < % .3f"
					", dtime_jitter = % .1f %%"
					", v_range = %.1f)",
					player_position.X/BS,
					player_position.Y/BS,
					player_position.Z/BS,
					busytime_jitter1_min_sample,
					busytime_jitter1_max_sample,
					dtime_jitter1_max_fraction * 100.0,
					draw_control.wanted_range
					);

			guitext2->setText(narrow_to_wide(temptext).c_str());
		}
		
		{
			guitext_info->setText(infotext.c_str());
		}
		
		/*
			Get chat messages from client
		*/
		{
			// Get new messages
			std::wstring message;
			while(client.getChatMessage(message))
			{
				chat_lines.push_back(ChatLine(message));
				/*if(chat_lines.size() > 6)
				{
					core::list<ChatLine>::Iterator
							i = chat_lines.begin();
					chat_lines.erase(i);
				}*/
			}
			// Append them to form the whole static text and throw
			// it to the gui element
			std::wstring whole;
			// This will correspond to the line number counted from
			// top to bottom, from size-1 to 0
			s16 line_number = chat_lines.size();
			// Count of messages to be removed from the top
			u16 to_be_removed_count = 0;
			for(core::list<ChatLine>::Iterator
					i = chat_lines.begin();
					i != chat_lines.end(); i++)
			{
				// After this, line number is valid for this loop
				line_number--;
				// Increment age
				(*i).age += dtime;
				/*
					This results in a maximum age of 60*6 to the
					lowermost line and a maximum of 6 lines
				*/
				float allowed_age = (6-line_number) * 60.0;

				if((*i).age > allowed_age)
				{
					to_be_removed_count++;
					continue;
				}
				whole += (*i).text + L'\n';
			}
			for(u16 i=0; i<to_be_removed_count; i++)
			{
				core::list<ChatLine>::Iterator
						it = chat_lines.begin();
				chat_lines.erase(it);
			}
			guitext_chat->setText(whole.c_str());

			// Update gui element size and position

			/*core::rect<s32> rect(
					10,
					screensize.Y - guitext_chat_pad_bottom
							- text_height*chat_lines.size(),
					screensize.X - 10,
					screensize.Y - guitext_chat_pad_bottom
			);*/
			core::rect<s32> rect(
					10,
					50,
					screensize.X - 10,
					50 + text_height*chat_lines.size()
			);

			guitext_chat->setRelativePosition(rect);

			if(chat_lines.size() == 0)
				guitext_chat->setVisible(false);
			else
				guitext_chat->setVisible(true);
		}

		/*
			Inventory
		*/
		
		static u16 old_selected_item = 65535;
		if(client.getLocalInventoryUpdated()
				|| g_selected_item != old_selected_item)
		{
			old_selected_item = g_selected_item;
			//std::cout<<"Updating local inventory"<<std::endl;
			client.getLocalInventory(local_inventory);
		}
		
		/*
			Send actions returned by the inventory menu
		*/
		while(inventory_action_queue.size() != 0)
		{
			InventoryAction *a = inventory_action_queue.pop_front();

			client.sendInventoryAction(a);
			// Eat it
			delete a;
		}

		/*
			Drawing begins
		*/

		TimeTaker drawtimer("Drawing");

		
		{
			TimeTaker timer("beginScene");
			driver->beginScene(true, true, bgcolor);
			//driver->beginScene(false, true, bgcolor);
			beginscenetime = timer.stop(true);
		}

		//timer3.stop();
		
		//std::cout<<DTIME<<"smgr->drawAll()"<<std::endl;
		
		{
			TimeTaker timer("smgr");
			smgr->drawAll();
			scenetime = timer.stop(true);
		}
		
		{
		//TimeTaker timer9("auxiliary drawings");
		// 0ms
		
		//timer9.stop();
		//TimeTaker //timer10("//timer10");
		
		video::SMaterial m;
		//m.Thickness = 10;
		m.Thickness = 3;
		m.Lighting = false;
		driver->setMaterial(m);

		driver->setTransform(video::ETS_WORLD, core::IdentityMatrix);

		for(core::list< core::aabbox3d<f32> >::Iterator i=hilightboxes.begin();
				i != hilightboxes.end(); i++)
		{
			/*std::cout<<"hilightbox min="
					<<"("<<i->MinEdge.X<<","<<i->MinEdge.Y<<","<<i->MinEdge.Z<<")"
					<<" max="
					<<"("<<i->MaxEdge.X<<","<<i->MaxEdge.Y<<","<<i->MaxEdge.Z<<")"
					<<std::endl;*/
			driver->draw3DBox(*i, video::SColor(255,0,0,0));
		}

		/*
			Frametime log
		*/
		if(g_settings.getBool("frametime_graph") == true)
		{
			s32 x = 10;
			for(core::list<float>::Iterator
					i = frametime_log.begin();
					i != frametime_log.end();
					i++)
			{
				driver->draw2DLine(v2s32(x,50),
						v2s32(x,50+(*i)*1000),
						video::SColor(255,255,255,255));
				x++;
			}
		}
#if 0
		/*
			Draw map plot
		*/
		if(g_show_map_plot && g_map_plot_texture)
		{
			core::dimension2d<u32> drawdim(640,480);
			core::rect<s32> dest(v2s32(0,0), drawdim);
			dest += v2s32(
				(screensize.X-drawdim.Width)/2,
				(screensize.Y-drawdim.Height)/2
			);
			core::rect<s32> source(v2s32(0,0), g_map_plot_texture->getSize());
			driver->draw2DImage(g_map_plot_texture, dest, source);
		}
#endif
		/*
			Draw crosshair
		*/
		driver->draw2DLine(displaycenter - core::vector2d<s32>(10,0),
				displaycenter + core::vector2d<s32>(10,0),
				video::SColor(255,255,255,255));
		driver->draw2DLine(displaycenter - core::vector2d<s32>(0,10),
				displaycenter + core::vector2d<s32>(0,10),
				video::SColor(255,255,255,255));

		} // timer

		//timer10.stop();
		//TimeTaker //timer11("//timer11");

		/*
			Draw gui
		*/
		// 0-1ms
		guienv->drawAll();

		/*
			Draw hotbar
		*/
		{
			draw_hotbar(driver, font, v2s32(displaycenter.X, screensize.Y),
					hotbar_imagesize, hotbar_itemcount, &local_inventory);
		}
		
		// End drawing
		{
			TimeTaker timer("endScene");
			driver->endScene();
			endscenetime = timer.stop(true);
		}

		drawtime = drawtimer.stop(true);

		/*
			End of drawing
		*/

#if 0
		/*
			Refresh map plot if player has moved considerably
		*/
		if(g_refresh_map_plot)
		{
			static v3f old_player_pos = v3f(1,1,1) * 10000000;
			v3f p = client.getPlayerPosition() / BS;
			if(old_player_pos.getDistanceFrom(p) > 4 * g_map_plot_texture_scale)
			{
				updateMapPlotTexture(v2f(p.X,p.Z), driver, &client);
				old_player_pos = p;
			}
			g_refresh_map_plot = false;
		}
#endif
		
		static s16 lastFPS = 0;
		//u16 fps = driver->getFPS();
		u16 fps = (1.0/dtime_avg1);

		if (lastFPS != fps)
		{
			core::stringw str = L"Minetest [";
			str += driver->getName();
			str += "] FPS:";
			str += fps;

			device->setWindowCaption(str.c_str());
			lastFPS = fps;
		}
		
		/*}
		else
			device->yield();*/
	}

	//delete quick_inventory;

	/*
		Disable texture fetches and other stuff that is queued
		to be processed by the main loop.

		This has to be done before client goes out of scope.
	*/
	g_irrlicht->Shutdown(true);

	} // client and server are deleted at this point

	} //try
	catch(con::PeerNotFoundException &e)
	{
		dstream<<DTIME<<"Connection timed out."<<std::endl;
		error_message = L"Connection timed out.";
	}

	} // Menu-game loop
	
	delete g_input;

	/*
		In the end, delete the Irrlicht device.
	*/
	device->drop();
	
	/*
		Update configuration file
	*/
	/*if(configpath != "")
	{
		g_settings.updateConfigFile(configpath.c_str());
	}*/

	END_DEBUG_EXCEPTION_HANDLER
	
	debugstreams_deinit();
	
	return 0;
}

//END
