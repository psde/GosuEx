DATA
====

Data is a easy-to-use package to handle serialization and compressing of data. To use this library it is necessary to compile it and on Windows %GOSU_DIR% has to be set.

TUTORIAL
=======

SAVING
------
Using it is really easy. Taking the Gosu Tutorial game for serialization (=> saving)
    // Used for serialization/deserialization
    #include <GosuEx/Data/Packet.hpp>
    // Used for compressing/decompressing the savegame
    #include <GosuEx/Data/Compress.hpp>
    
    // Serialize the player
    void Player::serialize(GosuEx::Data::Packet& p) {
      p << posX << posY << velX << velY << angle << score;
    }
    
    // Serialize a star
    void Star::serialize(GosuEx::Data::Packet& p) {
      p << posX << posY;
      // RGB
      p << color.red() << color.green() << color.blue();
    }
    
    // This function will tell the objects to "serialize themselves"
    void GameWindow::saveGame() {
      // Create a new "Packet"
      GosuEx::Data::Packet p;
      // Now we're ready to put our stuff into it
      // You may want to insert some kind of version for further (in)validation of savegames
      // We don't do this here, though
      player.serialize(p);
      // Add the size of the stars. C cast only to make sure it's that type.
      p << (std::size_t)stars.size();
      
      // For every star
      for (std::list<Star>::iterator it = stars.begin(); it != stars.end(); ++it) {
        star.serialize(p);
      }
      
      // Serialized!
      // Now we can compress the archive.
      // We will store the compressed data here
      Gosu::Buffer buf;
      // "default compression"
      // Don't do exception handling here
      // at least not yet.
      //! TODO.
      GosuEx::Data::deflatePacket(p, buf);
      // Save the buffer to a file.
      Gosu::safeFile(buf, Gosu::resourcePrefix()+L"savegame.sav");
      // Done.
    }
Pretty easy? Some things to notice:
1. This will ONLY save. Loading the thing again is a bit more complicated and explained below
2. You can not throw normal classes into a packet using <<. It does only understand the native types (int, float, double, bool, char.. you get it). Also, there are special cases for std::string and std::wstring. As you can see I'm adding the size of the vector as std::size_t so I can read how many stars are in there.
3. Make sure you add it in the same order as you get it again. Unlike a GosuEx::Data::Archive, it does not save it on a per-key basis.

LOADING
-------

Now, let's add loading.
    // Deserialize.
    void Player::deserialize(GosuEx::Data::Packet& p) {
      p >> posX >> posY >> velX >> velY >> angle >> score;
    }
    
    // Alternate constructor
    explicit Star::Star(Animation& anim, GosuEx::Data::Packet& p) {
      p >> posX >> posY;
      Gosu::Channel r, g, b;
      p >> r >> g >> b;
      color.setRed(r);
      color.setGreen(g);
      color.setBlue(b);
    }
    
    // Called when the player wants to load the game
    void GameWindow::loadGame() {
      // The packet we will later get the data from
      GosuEx::Data::Packet p;
      // The buffer we're loading the savegame in
      Gosu::Buffer buf;
      // Loading it. (again, no error exception. We assume it exists and is valid)
      Gosu::loadFile(buf, Gosu::resourcePrefix()+L"savegame.sav");
      // No error exception, default values.
      GosuEx::Data::inflatePacket(buf, p);
      // Now we can start.
      // Since player is kinda "hardcoded", we tell him to de-serialize.
      player.deserialize(p);
      // Delete ALL stars.
      stars.clear();
      // We need to know how many stars there are.
      std::size_t starsc;
      p >> starsc;
      for (std::size_t i = 0; i < starsc; i++) {
        stars.push_back(Star(starAnim, p));
      }
      // Done.
    }

And with this code we've loaded the savegame again. Sounds pretty easy? It is.

ARCHIVE
-------

Another part of Data is Archive. Archive is quite like a virtual folder, directory: You can add files with any name you want and read them again. You can only store things as a Gosu::Buffer. You should not compress them using GosuEx::Data::deflate (because Archives are compressed when saved). The name does not matter, as long as you use the same again.

Quick guide:
    #include <GosuEx/Data/Archive.hpp>
    
    GosuEx::Data::Archive ar;
    // Add a buffer to it
    ar.add(L"myBuffer", buffer);
    // Or load a file and put it into the archive.
    ar.addFile(L"myBufferFile", Gosu::resourcePrefix()+L"buffer.file");
    // Let's add the same buffer under another name
    ar.add(L"buffers/realbuffers/lololo/buffer.baf", buffer);
    
    // Save it. Default parameters, no exception check.
    ar.save(Gosu::resourcePrefix()+L"archive.arc");
    
    // Now, we open this archive. Either with .load() or just as a constructor.
    GosuEx::Data::Archive ar2(Gosu::resourcePrefix()+L"archive.arc");
    // Save myBuffer to a different file and remove it from ar2.
    // Notice: This won't affect the buffer in the stored archive or ar. Just in ar2.
    ar2.saveFile(L"myBuffer", Gosu::resourcePrefix()+L"myBuf.fer");
    ar2.remove(L"myBuffer");
    // This should not explode.
    assert(!ar2.exists(L"myBuffer"));
    // Iterate through all buffers.
    for (GosuEx::Data::Archive::iterator it = ar2.begin(); it != ar2.end(); it++) {
      // Print what we have.
      std::cout << "ar2 got " << *it;
      // Get the buffer
      Gosu::Buffer& buf = ar2.get(*it);
      std::cout << " with a size of " << buf.size() << std::endl;
    }