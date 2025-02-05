#include "reader.h"
#include "clip.h"
#include "bundle.h"

namespace mutant
{

std::string binary_input_utils::readString()
{
	std::string str;
	return readString( str );
}

std::string& binary_input_utils::readString( std::string& str )
{
	unsigned char c;
	int rd = 0;
	unsigned size = readDword();

	unsigned pos = str.size();
	str.resize( pos + size );
	std::fill_n( str.begin() + pos, size, ' ' );
	while( size-- ) {
		mInput->read( &c, sizeof(c), &rd );
		if( !rd )
			break;
		str[pos++] = c;
	}
	return str;
}


mutant_reader::mutant_reader( std::auto_ptr<binary_input> input )
:	binary_input_utils( input )
{
}

mutant_reader::~mutant_reader()
{
}

void mutant_reader::read( anim_character_set& char_set )
{
	try
	{
		// animations count, may be null
		unsigned int char_count = readDword();
		while( char_count-- )
		{
			std::auto_ptr<anim_character> character( new anim_character );

			std::string char_name = readCharacter( *character );

			char_set.insertCharacter(
				char_name,
				character
			);
		}
	} catch( EIoEof& ) {
		mutant_throw( "Unexpected end-of-file (file may be corrupted)" );
	} catch( EIoError& ) {
		mutant_throw( "Read/write error" );
	}
}

/*
void mutant_reader::read( data::base_mesh& mesh )
{
	try
	{
		mesh.vertexCount = readDword();
		mesh.vertexDataSize = readDword();
		mesh.vertexStride = readDword();
		mesh.vertexData = new unsigned char[mesh.vertexDataSize];
		readArray( mesh.vertexData, mesh.vertexDataSize );

		mesh.indexCount = readDword();
		mesh.indexSize = readDword();
		mesh.indexData = new unsigned char[mesh.indexCount * mesh.indexSize];
		readArray( mesh.indexData, mesh.indexCount * mesh.indexSize );

	} catch( EIoEof& ) {
		mutant_throw( "Unexpected end-of-file (file may be corrupted)" );
	} catch( EIoError& ) {
		mutant_throw( "Read/write error" );
	}
}

void mutant_reader::read( data::dx9_mesh& mesh )
{
	try
	{
		read( mesh.base() );
		mesh.fvfVertexDecl = readDword();
		mesh.primitiveType = static_cast<D3DPRIMITIVETYPE>(readDword());
		if( readBool() )
		{
			mesh.skinInfo = new data::skin_info;
			read( *mesh.skinInfo );
		}
		else
			mesh.skinInfo = 0;

	} catch( EIoEof& ) {
		mutant_throw( "Unexpected end-of-file (file may be corrupted)" );
	} catch( EIoError& ) {
		mutant_throw( "Read/write error" );
	}
}

void mutant_reader::read( data::skin_info& skin )
{
	try
	{
		skin.weightsPerVertex = readDword();
		skin.boneCount = readDword();
		skin.bones = new data::skin_info::Bone[skin.boneCount];
		for( size_t q = 0; q < skin.boneCount; ++q )
		{
			readType( skin.bones[q].matrix );
			readString( skin.bones[q].name );
		}
	} catch( EIoEof& ) {
		mutant_throw( "Unexpected end-of-file (file may be corrupted)" );
	} catch( EIoError& ) {
		mutant_throw( "Read/write error" );
	}
}
*/
/*namespace {
	bool autoRef( std::string const& names, simple_scene::Ref& ref, unsigned& refDataIt )
	{
		if(refDataIt >= names.size())
			return false;

		ref.data = (void*)&names[refDataIt];
		while( names[refDataIt] != 0x00 && refDataIt < names.size());
		return true;
	}

	size_t autoRefs( std::string const& names, simple_scene::Ref* refs, size_t count )
	{
		size_t refIt = 0;
		size_t refDataIt = 0;
		while( autoRef( names, refs[refIt], refDataIt ) )
			++refIt;

		for(size_t nullRefIt = refIt; nullRefIt < count; ++nullRefIt)
			refs[nullRefIt].data = 0;
		
		assert(refIt == count);
		return refIt;
	}

	void fixupRef( std::string const& names, simple_scene::Ref& ref )
	{
		unsigned offset = (unsigned)ref.data;
		ref.data = (void*)&names[offset];
	}

	void fixupRefs( std::string const& names, simple_scene::Ref* refs, size_t count )
	{
		for(size_t q = 0; q < count; ++q)
			fixupRef( names, refs[q] );
	}

}*/

/*
void mutant_reader::read( simple_scene& scene )
{
	try
	{
//		scene.meshIds = readString();
//		scene.clipIds = readString();
//		scene.nodeNames = readString();
//
//		size_t nodeNamesRefDataIt = 0;

		// meshes
		scene.meshCount = readDword();
		scene.meshIds = new simple_scene::Ref[scene.meshCount];
		readArray( scene.meshIds, scene.meshCount );
//		fixupRefs( scene.meshIds, scene.meshes, scene.meshCount );

		// lights
		scene.lightCount = readDword();
		scene.lights = new simple_scene::Light[scene.lightCount];
		for( size_t q = 0; q < scene.lightCount; ++q )
		{
//			readType( scene.lights[q].nodeName );
//			fixupRef( scene.nodeNames, scene.lights[q].nodeName );
			readString( scene.lights[q].nodeName );
			readType( scene.lights[q].worldMatrix );
		}

		// cameras
		scene.cameraCount = readDword();
		scene.cameras = new simple_scene::Camera[scene.cameraCount];
		for( size_t q = 0; q < scene.cameraCount; ++q )
		{
//			readType( scene.cameras[q].nodeName );
//			fixupRef( scene.nodeNames, scene.cameras[q].nodeName );
			readString( scene.cameras[q].nodeName );
			readType( scene.cameras[q].worldMatrix );
		}
		scene.defaultCameraIndex = readDword();

		// actors
		scene.actorCount = readDword();
		scene.actors = new simple_scene::Actor[scene.actorCount];
		for( size_t q = 0; q < scene.actorCount; ++q )
		{
//			readType( scene.actors[q].nodeName );
//			fixupRef( scene.nodeNames, scene.actors[q].nodeName );
			readString( scene.actors[q].nodeName );
			readType( scene.actors[q].worldMatrix );
			scene.actors[q].meshIndex = readDword();
		}

		// clips
//		scene.clipCount = readDword();
//		scene.clips = new simple_scene::Ref[scene.clipCount];
//		readArray( scene.clips, scene.clipCount );
//		fixupRefs( scene.clipIds, scene.clips, scene.clipCount );
		readString( scene.animCharId );
		scene.defaultClipIndex = readDword();

	} catch( EIoEof& ) {
		mutant_throw( "Unexpected end-of-file (file may be corrupted)" );
	} catch( EIoError& ) {
		mutant_throw( "Read/write error" );
	}
}
*/
std::string mutant_reader::readCharacter( anim_character& anim_char )
{
		std::string char_name = readString();

	unsigned int hier_count = readDword();
	unsigned int clip_count = readDword();	

	mutlog << "[mutant]: reading character `" << char_name << "' with " << clip_count << " clips, " << hier_count << " hierarchies\n";

	while( hier_count-- )
	{
		std::auto_ptr<anim_hierarchy> hierarchy( new anim_hierarchy );
		readHierarchy( *hierarchy );

		anim_char.insert_hierarchy( hierarchy );
	}

	while( clip_count-- )
	{
		std::auto_ptr<anim_clip> clip( new anim_clip );
		std::string clip_name = readClip( *clip );

		anim_char.insertClip( clip_name, clip );
	}

	return char_name;
}

void mutant_reader::readHierarchy( anim_hierarchy& hier )
{
	std::string hier_name = readString();
	unsigned node_count = readDword();

	if( node_count <= 0 )
		mutlog << "[mutant]: WARNING! hierarchy `" << hier_name << "' is empty (has " << node_count << " nodes)\n";
	else
		mutlog << "[mutant]: reading hierarchy `" << hier_name << "' with " << node_count << " nodes\n";

	hier.setName( hier_name );

	while( node_count-- )
	{
		hier.nodes().push_back( anim_hierarchy::node_t() );
		anim_hierarchy::node& node = hier.nodes().back();

		node.name = readString();

		unsigned child_count = readDword();
		node.children.resize( child_count );
		//readVector( node.children, child_count );
		//readData(node.children.begin(), node.children.end());
		readArray(node.children.begin(), node.children.end());
	}

	if( hier.size() > 0 )
		anim_hierarchy::node_t::update_parent_all( hier );
}

std::string mutant_reader::readClip( anim_clip& clip )
{
	std::string clip_name = readString();
	clip.set_flags( readDword() );
	float clipLength = 0.0f; readType( clipLength ); clip.set_length( clipLength );
	unsigned int bundle_count = readDword();

	mutlog << "[mutant]: reading clip `" << clip_name << "' with " << bundle_count << " bundle(s)\n";

	while( bundle_count-- )
	{
		std::auto_ptr<anim_bundle> bundle( new anim_bundle );
		std::string bundle_name = readBundle( *bundle, clip );

		clip.insertBundle( bundle_name, bundle );
	}

	return clip_name;
}

std::string mutant_reader::readBundle( anim_bundle& bundle, anim_clip& clip )
{
	std::string bundle_name = readString();
	unsigned int anims_count = readDword();

	mutlog << "[mutant]: reading bundle `" << bundle_name << "' with " << anims_count << " anim(s)\n";

	while( anims_count-- )
	{
		readAnimation( bundle, clip );
	}

	return bundle_name;
}


void mutant_reader::readAnimation( anim_bundle& bundle, anim_clip& clip )
{
	typedef std::vector<float> float_values_vec_t;
	typedef std::vector<std::string> string_values_vec_t;

	eAnimType element_type = (eAnimType)readByte();
	unsigned char component_size = readByte();
	std::string anim_type;
	readString(anim_type);
	unsigned int key_count = readDword();

	assert( element_type >= ANIM_UNKNOWN && element_type < ANIM_MAX );

	switch( element_type )
	{
	case ANIM_FLOAT:
		{
			std::auto_ptr<float_values_vec_t> keys( new float_values_vec_t );
			std::auto_ptr<float_values_vec_t> values( new float_values_vec_t );

			keys->resize( key_count );
			values->resize( key_count * component_size );

			//readVector( *keys, key_count );
			//readVector( *values, key_count * component_size );
			readData(keys->begin(), keys->end());
			readData(values->begin(), values->end());

			bundle.insertData(
				anim_type,
				knot_data<float,float>( *keys, *values, component_size )
			);

			clip.registerData( keys );
			clip.registerData( values );

			break;
		}
	case ANIM_STRING:
		{
			std::auto_ptr<float_values_vec_t> keys( new float_values_vec_t );
			std::auto_ptr<string_values_vec_t> values( new string_values_vec_t );

			keys->resize( key_count );
			values->resize( key_count * component_size );

			//readVector( *keys, key_count );
			readData(keys->begin(), keys->end());
			readVector(*values, key_count * component_size);

			bundle.insertData(
				anim_type,
				knot_data<float,std::string>( *keys, *values, component_size )
			);

			clip.registerData( keys );
			clip.registerData( values );

			break;
		}
	case ANIM_VECTOR3:
		default:
			mutant_throw( "mutant: Invalid animation element type" );
			return;
	}
}

}
