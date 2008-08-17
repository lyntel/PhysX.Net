#include "StdAfx.h"
#include "Controller Manager.h"
#include "Controller Description.h"
#include "Box Controller.h"
#include "Capsule Controller.h"
#include "Scene.h"
#include "Functions.h"

#include <NxBoxController.h> 
#include <NxCapsuleController.h> 

using namespace System;
using namespace StillDesign::PhysX;

StillDesign::PhysX::ControllerManager::ControllerManager( NxControllerManager* manager, StillDesign::PhysX::Scene^ scene )
{
	Debug::Assert( manager != NULL );
	Debug::Assert( scene != nullptr );
	
	ObjectCache::Add( (intptr_t)manager, this );
	
	_manager = manager;
	_scene = scene;
	
	_controllers = gcnew ElementCollection< Controller^, ControllerCollection^ >();
}
StillDesign::PhysX::ControllerManager::~ControllerManager()
{
	this->!ControllerManager();
}
StillDesign::PhysX::ControllerManager::!ControllerManager()
{
	if( this->IsDisposed == true )
		return;
	
	onDisposing( this, nullptr );
	
	_controllers->DiposeOfAll();
	
	NxReleaseControllerManager( _manager );
	_manager = NULL;
	
	_scene = nullptr;
	_controllers = nullptr;
	
	_userData = nullptr;
	
	onDisposed( this, nullptr );
}
bool StillDesign::PhysX::ControllerManager::IsDisposed::get()
{
	return ( _manager == NULL );
}

StillDesign::PhysX::Controller^ StillDesign::PhysX::ControllerManager::CreateController( ControllerDescription^ controllerDescription )
{
	if( controllerDescription == nullptr )
		throw gcnew ArgumentNullException( "controllerDescription" );
	
	if( controllerDescription->IsValid() == false )
		throw gcnew ArgumentException( "Controller description is invalid" );
	
	Controller^ controller;
	
	switch( controllerDescription->Type )
	{
		case ControllerType::Box:
		{
			NxBoxController* boxController = (NxBoxController*)_manager->createController( this->Scene->UnmanagedPointer, *controllerDescription->UnmanagedPointer );
			
			BoxController^ c = gcnew BoxController( boxController, this );
			
			controller = c;
		}
		break;
		case ControllerType::Capsule:
		{
			NxCapsuleController* capsuleController = (NxCapsuleController*)_manager->createController( this->Scene->UnmanagedPointer, *controllerDescription->UnmanagedPointer );
			
			CapsuleController^ c = gcnew CapsuleController( capsuleController, this );
			
			controller = c;
		}
		break;
		
		default: throw gcnew InvalidOperationException( "Unsupported controller type" );
	}
	
	_controllers->Add( controller );
	
	return controller;
}

void StillDesign::PhysX::ControllerManager::UpdateControllers()
{
	_manager->updateControllers();
}

StillDesign::PhysX::Scene^ StillDesign::PhysX::ControllerManager::Scene::get()
{
	return _scene;
}

StillDesign::PhysX::ControllerManager::ControllerCollection^ StillDesign::PhysX::ControllerManager::Controllers::get()
{
	return _controllers->ReadOnlyCollection;
}

String^ StillDesign::PhysX::ControllerManager::Name::get()
{
	return _name;
}
void StillDesign::PhysX::ControllerManager::Name::set( String^ value )
{
	_name = value;
}

Object^ StillDesign::PhysX::ControllerManager::UserData::get()
{
	return _userData;
}
void StillDesign::PhysX::ControllerManager::UserData::set( Object^ value )
{
	_userData = value;
}

NxControllerManager* StillDesign::PhysX::ControllerManager::UnmanagedPointer::get()
{
	return _manager;
}