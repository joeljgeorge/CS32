/*#include <iostream>
#include <string>
using namespace std;*/

class File {
public:
	File(string name) {
		file_name = name;
	}
	string name() const;
	virtual void open() const = 0;
	virtual void redisplay() const;
	virtual ~File();

private:
	string file_name;
};

class TextMsg : public File
{
public:
	TextMsg(string name);
	virtual void open() const;
	virtual ~TextMsg();

};

class Video : public File
{
public:
	Video(string name, int length);
	virtual void open() const;
	virtual void redisplay() const;
	virtual ~Video();
private:
	int video_length;
};

class Picture : public File
{
public: 
	Picture(string name);
	virtual void open() const;
	virtual ~Picture();
};

string File::name() const{
	return file_name;
}

//File function implementation
void File::redisplay() const{
	cout << "refresh the screen";
}

File::~File() {}

//TextMsg function implementation
TextMsg::TextMsg(string name) : File(name) {}

void TextMsg::open() const{
	cout << "open text message";
}

TextMsg::~TextMsg() {
	cout << "Destroying " << name() << ", a text message" << endl;
}

//Video function implementation
Video::Video(string name, int length) :File(name) {
	video_length = length;
}

void Video::open() const {
	cout << "play " << video_length << " second video";
}

void Video::redisplay() const {
	cout << "replay the video";
}

Video::~Video() {
	cout << "Destroying " << name() << ", a video" << endl;
}

//Video function implementation
Picture::Picture(string name): File(name) {}

void Picture::open() const{
	cout << "show picture"; 
}

Picture::~Picture() {
	cout << "Destroying the picture " << name() << endl;
}

void openAndRedisplay(const File* f)
{
	cout << f->name() << ": ";
	f->open();
	cout << endl << "Redisplay: ";
	f->redisplay();
	cout << endl;
}
/*int main()
{
	File* files[4];
	files[0] = new TextMsg("fromFred.txt");
	// Videos have a name and running time
	files[1] = new Video("goblin.mpg", 3780);
	files[2] = new Picture("kitten.jpg");
	files[3] = new Picture("baby.jpg");
	for (int k = 0; k < 4; k++)
		openAndRedisplay(files[k]);

	// Clean up the files before exiting
	cout << "Cleaning up." << endl;
	for (int k = 0; k < 4; k++)
		delete files[k];
}*/
