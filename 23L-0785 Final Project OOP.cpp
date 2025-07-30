#include<iostream>
#include<fstream>
#include<cstring>
using namespace std;

class Page;
class User;
class Post;
class Object;
class Date;
class Controller;
class Comment;
class PostContent;
class Activity;

//---------------------------------------CLASS "HELPER"--------------------------------------------------------------------
class Helper
{
public:
	static int MyStrLen(const char* str)
	{
		int lenght = 0;
		for (int i = 0; str[i] != '\0'; i++)
		{
			lenght++;
		}
		return lenght;
	}

	static void MyStrCpy(char*& dest, const char* src)
	{
		int i = 0;
		for (; src[i] != '\0'; i++)
		{
			dest[i] = src[i];
		}
		dest[i] = '\0';
	}

	static char* GetStringFromBuffer(char* buffer)
	{
		int strLen = MyStrLen(buffer);
		char* str = 0;
		if (strLen > 0)
		{
			str = new char[strLen + 1];
			MyStrCpy(str, buffer);
		}
		return str;
	}
	static char* GetStringFromBuffer(const char* buffer)
	{
		int strLen = MyStrLen(buffer);
		char* str = 0;
		if (strLen > 0)
		{
			str = new char[strLen + 1];
			MyStrCpy(str, buffer);
		}
		return str;
	}
};

template<typename T>
void MemoryDeallocation(T** arr)
{
	for (int i = 0; arr[i] != nullptr; i++)
	{
		delete[] arr[i];
	}
	delete[] arr;
}


//---------------------------------------CLASS "DATE"--------------------------------------------------------------------
class Date
{
private:
	int day;
	int month;
	int year;
public:
	static Date CurrentDate;
	Date()
	{
		day = 0;
		month = 0;
		year = 0;
	}
	static void SetCurrentDate(int d, int m, int y)
	{
		CurrentDate.day = d;
		CurrentDate.month = m;
		CurrentDate.year = y;
	}

	void PrintDate()
	{
		cout << "(" << day << "/" << month << "/" << year << ")";
		cout << "\n";
	}
	static void PrintCurrentDate()
	{
		cout << "(" << CurrentDate.day << "/" << CurrentDate.month << "/" << CurrentDate.year << ")";
		cout << "\n";
	}
	void ReadDataFromFile(ifstream& fin)
	{
		fin >> day >> month >> year;
	}

	bool CheckLatestDate()
	{
		if ((CurrentDate.day == day || CurrentDate.day - 1 == day || CurrentDate.day - 2 == day) && CurrentDate.month == month && CurrentDate.year == year)
		{
			return true;
		}
		return false;
	}

	void ComparingDate()
	{
		if (CurrentDate.day == day && CurrentDate.month == month && CurrentDate.year == year)
		{
			cout << "(1h):\n";
		}
		else if ((CurrentDate.day > day) && CurrentDate.month == month && CurrentDate.year == year)
		{
			cout << "(" << CurrentDate.day - day << "d):\n";
		}
		else
		{
			PrintDate();
		}
	}
	~Date()
	{

	}
};

Date Date::CurrentDate;

//---------------------------------------CLASS "OBJECT"--------------------------------------------------------------------
class Object
{
private:
	char* ID;
protected:
	Post** timeline;
	int total;
public:

	Object()
	{
		total = 0;
		ID = nullptr;
		timeline = new Post * [10];
		for (int i = 0; i < 10; i++)
		{
			timeline[i] = nullptr;
		}
	}

	~Object()
	{
		if (ID)
		{
			delete ID;
		}
	}

	void SetID(const char* id)
	{
		int lenght = Helper::MyStrLen(id);
		ID = new char[lenght + 1];
		Helper::MyStrCpy(ID, id);
	}

	char* GetID()
	{
		return ID;
	}

	virtual void PrintName() = 0;
	virtual void AddPostToTimeline(Post* ptr) = 0;
	virtual void ViewTimeline() = 0;
	void ReadDataFromFile(ifstream& fin)
	{
		char temp[50];
		fin >> temp;
		ID = Helper::GetStringFromBuffer(temp);
	}
};

//---------------------------------------CLASS "COMMENTS"--------------------------------------------------------------------
class Comment
{
private:
	Object* commentBy;
	const char* ID;
	const char* text;
	static int totalComments;
	Post* _post;
public:
	Comment()
	{
		commentBy = nullptr;
		text = nullptr;
		ID = nullptr;
		_post = nullptr;
	}
	~Comment()
	{
		if (text)
		{
			delete text;
		}
		if (ID)
		{
			delete ID;
		}
	}
	void SetID(const char* id)
	{
		ID = id;
	}
	void SetCommentBy(Object* ptr)
	{
		commentBy = ptr;
	}
	void SetText(const char* str)
	{
		text = str;
	}
	void DisplayComments()
	{
		cout << "\t\t\t";
		commentBy->PrintName();
		cout << ": \"" << text << "\"";
	}
	void AddingComment(const char* _id, Post* p, Object* author, const char* t)
	{
		ID = Helper::GetStringFromBuffer(_id);
		text = Helper::GetStringFromBuffer(t);
		commentBy = author;
		_post = p;
	}
};
int Comment::totalComments = 0;

//---------------------------------------CLASS "POSTCONTENT"--------------------------------------------------------------------
class PostContent
{
public:
	virtual void PrintActivity() = 0;
	virtual ~PostContent() = 0 {};
};


//---------------------------------------CLASS "POST"--------------------------------------------------------------------
class Post
{
private:
	char* ID;
	char* text;
	int likesCount;
	int totalComments;
	Date sharedDate;
	Object* sharedBy;
	Object** LikedBy;
	Comment** comments;
	PostContent* content;
public:
	Post()
	{
		likesCount = totalComments = 0;
		ID = text = nullptr;
		sharedBy = nullptr;
		LikedBy = nullptr;
		comments = nullptr;
		sharedDate = Date();
		content = nullptr;
	}

	~Post()
	{
		if (text)
		{
			delete text;
		}
		if (ID)
		{
			delete ID;
		}
		if (content)
		{
			delete content;
		}
		if (comments)
		{
			MemoryDeallocation(comments);
		}
	}


	void ReadDataFromFile(ifstream& fin)
	{
		char _id[15];
		fin >> _id;
		ID = Helper::GetStringFromBuffer(_id);

		sharedDate.ReadDataFromFile(fin);

		fin.ignore();
		char temp[100];
		fin.getline(temp, 100);
		text = Helper::GetStringFromBuffer(temp);
		int len = Helper::MyStrLen(text);
		text[len] = '\0';
	}
	void AddSharedBy(Object* str)
	{
		sharedBy = str;
	}

	PostContent* GetContent()
	{
		return content;
	}

	void AddLikedBy(Object* str)
	{
		if (!LikedBy)
		{
			LikedBy = new Object * [10];
			for (int i = 0; i < 10; i++)
			{
				LikedBy[i] = nullptr;
			}
		}
		LikedBy[likesCount] = str;
		likesCount++;
	}
	char* GetID()
	{
		return ID;
	}

	Date GetSharedDate()
	{
		return sharedDate;
	}

	void ViewLikesOnPost()
	{
		if (likesCount == 0)
		{
			cout << "This Post has no likes\n";
		}
		else
		{
			for (int i = 0; i < likesCount; i++)
			{
				cout << LikedBy[i]->GetID() << " - ";
				LikedBy[i]->PrintName();
				cout << "\n";
			}
		}
	}

	void postComment(Object* author, char* t)
	{
		Comment* temp = new Comment;
		const char* id = " ";
		temp->AddingComment(id, this, author, t);
		AddComment(temp);
	}

	void SetContent(PostContent* ptr)
	{
		content = ptr;
	}

	void AddComment(Comment* ptr)
	{
		if (totalComments == 0)
		{
			comments = new Comment * [10];
		}
		comments[totalComments] = ptr;
		totalComments++;
	}


	virtual void DisplayPost()
	{
		cout << "---";
		sharedBy->PrintName();
		if (content != nullptr)
		{
			content->PrintActivity();
		}
		sharedDate.ComparingDate();
		cout << "\t\t" << "\"" << text << "\"";
		cout << endl;
		for (int i = 0; i < totalComments; i++)
		{
			comments[i]->DisplayComments();
			cout << endl;
		}
	}
};




//---------------------------------------CLASS "PAGE"--------------------------------------------------------------------
class Page : public Object
{
private:
	char* title;
public:
	Page()
	{
		title = nullptr;
	}
	void PrintName()
	{
		if (title)
		{
			cout << title;
		}
		else
		{
			cout << "Page information not available\n";
		}
	}
	void ReadDataFromFile(ifstream& fin)
	{
		char str[10];
		fin >> str;
		SetID(str);

		char temp[50];
		fin.getline(temp, 50);
		title = Helper::GetStringFromBuffer(temp);
	}
	void AddPostToTimeline(Post* ptr)
	{
		timeline[total] = ptr;
		total++;
	}
	void DisplayRecentPosts()
	{
		if (total)
		{
			for (int i = 0; i < total; i++)
			{
				Date obj = timeline[i]->GetSharedDate();
				if (obj.CheckLatestDate())
				{
					timeline[i]->DisplayPost();
				}
			}
		}
	}
	void ViewTimeline()
	{
		for (int i = 0; i < total; i++)
		{
			timeline[i]->DisplayPost();
		}

	}
	~Page()
	{
		if (title)
		{
			delete[] title;
		}
	}
};

//---------------------------------------CLASS "USER"--------------------------------------------------------------------
class User : public Object
{
private:
	char* fName;
	char* lName;
	User** friendList;
	Page** likedPages;
	int friendsCount;
	int likedPagesCount;
public:
	User();
	void PrintName();
	void ReadDataFromFile(ifstream& fin);
	void AddFriend(User*&);
	void LikePage(Page*&);
	void DisplayFriendList();
	void DisplayLikedPages();
	void AddPostToTimeline(Post* ptr);
	void ViewTimeline();
	void DisplayRecentPosts();
	void ViewHome();
	~User();
};

//---------------------------------------FUNCTIONS FOR CLASS "USER"--------------------------------------------------------------------

User::User()
{
	fName = lName = nullptr;
	friendList = nullptr;
	likedPages = nullptr;
	friendsCount = likedPagesCount = 0;
}
User::~User()
{
	if (fName)
	{
		delete[] fName;
	}
	if (lName)
	{
		delete[] lName;
	}
	if (friendList)
	{
		delete[] friendList;
	}
	if (likedPages)
	{
		delete[] likedPages;
	}

}

void User::PrintName()
{
	if (fName && lName)
	{
		cout << fName << " " << lName;
	}
	else
	{
		cout << "User information not available";
	}
}

void User::ReadDataFromFile(ifstream& fin)
{
	char str[50];
	fin >> str;
	SetID(str);

	fin >> str;
	fName = Helper::GetStringFromBuffer(str);

	fin >> str;
	lName = Helper::GetStringFromBuffer(str);
}

void User::AddFriend(User*& _friend)
{
	User** list = new User * [10];
	for (int i = 0; i < friendsCount; i++)
	{
		list[i] = friendList[i];
	}
	list[friendsCount++] = _friend;
	friendList = list;
}

void User::LikePage(Page*& _page)
{
	Page** list = new Page * [10];
	for (int i = 0; i < likedPagesCount; i++)
	{
		list[i] = likedPages[i];
	}
	list[likedPagesCount++] = _page;
	likedPages = list;
}

void User::AddPostToTimeline(Post* ptr)
{
	timeline[total] = ptr;
	total++;
}

void User::DisplayFriendList()
{
	for (int i = 0; i < friendsCount; i++)
	{
		cout << "\t" << i + 1 << ") " << friendList[i]->GetID() << " - ";
		friendList[i]->PrintName();
		cout << "\n";
	}
}

void User::DisplayLikedPages()
{
	for (int i = 0; i < likedPagesCount; i++)
	{
		cout << "\t" << i + 1 << ") " << likedPages[i]->GetID() << " -";
		likedPages[i]->PrintName();
		cout << "\n";
	}
}

void User::ViewTimeline()
{
	for (int i = 0; i < total; i++)
	{
		timeline[i]->DisplayPost();
	}
}

void User::DisplayRecentPosts()
{
	if (total)
	{
		for (int i = 0; i < total; i++)
		{
			Date obj = timeline[i]->GetSharedDate();
			if (obj.CheckLatestDate())
			{
				timeline[i]->DisplayPost();
			}
		}
	}
}

void User::ViewHome()
{
	if (total)
	{
		for (int i = 0; i < total; i++)
		{
			Date current = timeline[i]->GetSharedDate();
			if (current.CheckLatestDate())
			{
				timeline[i]->DisplayPost();
			}
		}
	}
	if (friendsCount)
	{
		for (int i = 0; i < friendsCount; i++)
		{
			friendList[i]->DisplayRecentPosts();
		}
	}
	if (likedPagesCount)
	{
		for (int i = 0; i < likedPagesCount; i++)
		{
			likedPages[i]->DisplayRecentPosts();
		}
	}
}

//---------------------------------------CLASS "ACTIVITY"--------------------------------------------------------------------
class Activity : public PostContent
{
private:
	int type;
	char* value;
public:
	Activity()
	{
		type = 0;
		value = nullptr;
	}

	void PrintActivity()
	{
		switch (type)
		{
		case 1:
			cout << " is Feeling " << value << " ";
			break;
		case 2:
			cout << " is Thinking about " << value << " ";
			break;
		case 3:
			cout << " is Making  " << value << " ";
			break;
		case 4:
			cout << " is Celebrating " << value << " ";
			break;
		default:
			cout << " ";
			break;
		}
	}

	void SetType(int t)
	{
		type = t;
	}

	void SetValue(const char* Val)
	{
		if (value != nullptr)
		{
			delete[] value;
		}
		int lenght = Helper::MyStrLen(Val);
		value = new char[lenght + 1];
		Helper::MyStrCpy(value, Val);
	}
	~Activity()
	{
		if (value)
		{
			delete[] value;
		}
	}
};

//---------------------------------------CLASS "CONTROLLER"--------------------------------------------------------------------
class Controller
{
private:
	User** AllUsers;
	Page** AllPages;
	Post** AllPosts;
	Activity** AllActivities;
	int usersCount;
	int pagesCount;
	int postsCount;
	int commentsCount;
	int activitiesCount;
public:
	Controller();
	User* SearchUserByID(const char*);
	Page* SearchPageByID(const char*);
	Post* SearchPostByID(const char*);
	Object* SearchObjByID(const char*);
	void WriteComment(Post*&, char*, Object*);
	void LoadAllUsers(const char*);
	void LoadAllPages(const char*);
	void LinkUsersAndPages(const char*);
	void LoadAllPosts(const char*);
	void LoadAllComments(const char*);
	void LoadAllActivities(const char*);
	void PrintPost(const char*);
	void LoadData();
	~Controller();
};

//---------------------------------------FUNCTIONS FOR CLASS "CONTROLLER"--------------------------------------------------------------------

Controller::Controller()
{
	AllPages = nullptr;
	AllUsers = nullptr;
	AllPosts = nullptr;
	AllActivities = nullptr;
	usersCount = pagesCount = postsCount = commentsCount = activitiesCount = 0;
}

Controller::~Controller()
{
	

}

User* Controller::SearchUserByID(const char* id)
{
	for (int i = 0; i < usersCount; i++)
	{
		if (strcmp(AllUsers[i]->GetID(), id) == 0)
		{
			return AllUsers[i];
		}
	}
	return NULL;
}

Page* Controller::SearchPageByID(const char* id)
{
	for (int i = 0; i < pagesCount; i++)
	{
		if (strcmp(AllPages[i]->GetID(), id) == 0)
		{
			return AllPages[i];
		}
	}
	return NULL;
}


Post* Controller::SearchPostByID(const char* id)
{
	for (int i = 0; i < postsCount; i++)
	{
		if (strcmp(AllPosts[i]->GetID(), id) == 0)
		{
			return AllPosts[i];
		}
	}
	return NULL;
}

Object* Controller::SearchObjByID(const char* id)
{
	if (id[0] == 'p')
	{
		Object* pagePtr = SearchPageByID(id);
		return pagePtr;
	}
	else if (id[0] == 'u')
	{
		Object* userPtr = SearchUserByID(id);
		return userPtr;
	}
	else
	{
		return nullptr;
	}
}

void Controller::LoadAllUsers(const char* file)
{
	ifstream fin(file);
	if (!fin.is_open())
	{
		cout << "ERROR OPENING FILE!!!\n";
	}

	fin >> usersCount;
	AllUsers = new User * [usersCount];
	for (int i = 0; i < usersCount; i++)
	{
		AllUsers[i] = new User();
		AllUsers[i]->ReadDataFromFile(fin);
	}
	fin.close();
}

void Controller::LoadAllPages(const char* file)
{
	ifstream fin(file);
	if (!fin.is_open())
	{
		cout << "ERROR OPENING FILE!!!\n";
	}

	fin >> pagesCount;
	AllPages = new Page * [pagesCount];
	for (int i = 0; i < pagesCount; i++)
	{
		AllPages[i] = new Page();
		AllPages[i]->ReadDataFromFile(fin);
	}
	fin.close();
}

void Controller::LinkUsersAndPages(const char* filename)
{
	ifstream fin(filename);
	if (!fin.is_open())
	{
		cout << "Error opening file: " << filename << endl;
		return;
	}

	char Id[10];
	while (fin >> Id && strcmp(Id, "-1") != 0)
	{
		User* current = SearchUserByID(Id);
		if (current == nullptr)
		{
			continue;
		}
		char IdFriend[10];
		while (fin >> IdFriend && strcmp(IdFriend, "-1") != 0)
		{
			User* currentFriend = SearchUserByID(IdFriend);
			if (currentFriend)
			{
				current->AddFriend(currentFriend);
			}
		}
		char pageId[10];
		while (fin >> pageId && strcmp(pageId, "-1") != 0)
		{
			Page* CurrentPage = SearchPageByID(pageId);
			if (CurrentPage != nullptr)
			{
				current->LikePage(CurrentPage);
			}
		}
	}
	fin.close();
}

void Controller::LoadAllPosts(const char* filename)
{
	ifstream fin(filename);
	if (!fin.is_open())
	{
		cout << "Error opening file: " << filename << endl;
		return;
	}
	fin >> postsCount;
	AllPosts = new Post * [postsCount];
	for (int i = 0; i < postsCount; i++)
	{
		AllPosts[i] = new Post();
		AllPosts[i]->ReadDataFromFile(fin);

		char data[80];
		fin >> data;
		Object* temp = SearchObjByID(data);
		if (temp != nullptr)
		{
			AllPosts[i]->AddSharedBy(temp);
			temp->AddPostToTimeline(AllPosts[i]);
		}
		char data2[80];
		while (fin >> data2 && strcmp(data2, "-1") != 0)
		{
			Object* temp2 = SearchObjByID(data2);
			if (temp2 != nullptr)
			{
				AllPosts[i]->AddLikedBy(temp2);
			}
		}
	}
	fin.close();
}

void Controller::LoadAllComments(const char* filename)
{
	ifstream fin(filename);
	if (!fin.is_open())
	{
		cout << "Error opening file: " << filename << endl;
		return;
	}
	fin >> commentsCount;
	fin.ignore();
	for (int i = 0; i < commentsCount; i++)
	{
		Comment* ptr = new Comment();

		char id[10];
		fin >> id;
		ptr->SetID(id);

		char id_post[10];
		fin >> id_post;
		Post* postPtr = SearchPostByID(id_post);
		if (postPtr != nullptr)
		{
			postPtr->AddComment(ptr);
		}

		char objID[20];
		fin >> objID;
		Object* commentedby = SearchObjByID(objID);
		if (commentedby != nullptr)
		{
			ptr->SetCommentBy(commentedby);
		}

		char textOfComment[80];
		fin.getline(textOfComment, 80);
		ptr->SetText(Helper::GetStringFromBuffer(textOfComment));

	}
	fin.close();

}

void Controller::LoadAllActivities(const char* filename)
{
	ifstream fin(filename);
	if (!fin.is_open())
	{
		cout << "Error opening file: " << filename << endl;
		return;
	}
	fin >> activitiesCount;
	AllActivities = new Activity * [10];
	for (int i = 0; i < activitiesCount; i++)
	{
		AllActivities[i] = new Activity();
		char id[10];
		fin >> id;
		Post* id_ptr = SearchPostByID(id);

		int activityType;
		fin >> activityType;
		AllActivities[i]->SetType(activityType);

		char activityValue[80];
		fin.getline(activityValue, 80);
		AllActivities[i]->SetValue(activityValue);

		PostContent* ptr = AllActivities[i];
		if (id_ptr != nullptr)
		{

			id_ptr->SetContent(ptr);
		}
	}
	fin.close();
}

void Controller::PrintPost(const char* ptr)
{
	Post* ptr_post = SearchPostByID(ptr);
	if (ptr_post != nullptr)
	{
		ptr_post->DisplayPost();
	}
}

void Controller::WriteComment(Post*& p, char* t, Object* author)
{
	p->postComment(author, t);
}

void Controller::LoadData()
{
	LoadAllUsers("Users.txt");
	LoadAllPages("Pages.txt");
	LinkUsersAndPages("UserFriendsAndPages.txt");
	LoadAllPosts("SocialNetworkPosts.txt");
	LoadAllComments("SocialNetworksComments.txt");
	LoadAllActivities("SocialNetworkActivities.txt");
}

//---------------------------------------CLASS "SOCIAL-NETWORK-APP"--------------------------------------------------------------------
class SocialNetworkApp
{
public:
	void Run()
	{
		Controller MainApp;
		MainApp.LoadData();

		cout << "Command:\t\tSet current date system 17 4 2024\n";
		Date::SetCurrentDate(17, 4, 2024);
		cout << "System Date:\t\t";
		Date current;
		current.PrintCurrentDate();

		User* displayUser = MainApp.SearchUserByID("u7");
		if (displayUser)
		{
			cout << "--------------------------------------------------------------------------------------------\n";
			cout << "Command:\t\tSet current user \"u7\"\n";
			displayUser->PrintName();
			cout << " has been successfully set as current user\n";

			cout << "--------------------------------------------------------------------------------------------\n";
			cout << "Command:\t\tView Friend List\n";
			cout << "--------------------------------------------------------------------------------------------\n";
			displayUser->PrintName();
			cout << " - Friend List\n";
			displayUser->DisplayFriendList();
			cout << "\n";

			cout << "--------------------------------------------------------------------------------------------\n";
			cout << "Command:\t\tView Liked Pages\n";
			cout << "--------------------------------------------------------------------------------------------\n";
			displayUser->PrintName();
			cout << " - Liked Pages\n";
			displayUser->DisplayLikedPages();
			cout << "\n";

			cout << "--------------------------------------------------------------------------------------------\n";
			cout << "Command:\t\tView Timeline\n";
			cout << "--------------------------------------------------------------------------------------------\n";
			displayUser->PrintName();
			cout << " - Timeline\n";
			displayUser->ViewTimeline();
		}
		else
		{
			cout << "Failed to set current user.....User does not exists.\n";
		}

		Post* current_Post = MainApp.SearchPostByID("post5");
		if (current_Post)
		{
			cout << "--------------------------------------------------------------------------------------------\n";
			cout << "Command:\t\tView Liked Post (post5)\n";
			cout << "--------------------------------------------------------------------------------------------\n";
			cout << "Post Liked By :\n";
			current_Post->ViewLikesOnPost();
			cout << "--------------------------------------------------------------------------------------------\n";
			cout << "Command:\t\tLike Post (post5)\n";
			cout << "Command:\t\tView Liked Post (post5)\n";
			cout << "--------------------------------------------------------------------------------------------\n";
			Object* temp = MainApp.SearchObjByID("u7");
			if (temp)
			{
				current_Post->AddLikedBy(temp);
			}
			current_Post->ViewLikesOnPost();
		}
		cout << "--------------------------------------------------------------------------------------------\n";
		cout << "Command:\t\tView Page (p1)\n";
		cout << "--------------------------------------------------------------------------------------------\n";
		Page* current_page = MainApp.SearchPageByID("p1");
		if (current_page)
		{
			current_page->ViewTimeline();
		}

		cout << "--------------------------------------------------------------------------------------------\n";
		cout << "Command:\t\tView Home\n";
		cout << "--------------------------------------------------------------------------------------------\n";
		displayUser->PrintName();
		cout << " - Home Page\n";
		displayUser->ViewHome();

		char commentOnPost[10] = "post4";
		char comment[80] = "Good Luck for your Result";
		Post* newptr = MainApp.SearchPostByID(commentOnPost);
		cout << "--------------------------------------------------------------------------------------------\n";
		cout << "Command:\t\tPostComment(post4, Good Luck for your Result)\n";
		cout << "--------------------------------------------------------------------------------------------\n";
		if (newptr != nullptr)
		{
			MainApp.WriteComment(newptr, comment, displayUser);
		}
		MainApp.PrintPost(commentOnPost);
		cout << endl;

		char commentOnPost2[10] = "post8";
		char comment2[80] = "Thanks for the Wishes";
		Post* newptr2 = MainApp.SearchPostByID(commentOnPost2);
		cout << "--------------------------------------------------------------------------------------------\n";
		cout << "Command:\t\tPostComment(post8, Thanks for the Wishes)\n";
		cout << "--------------------------------------------------------------------------------------------\n";
		if (newptr2 != nullptr)
		{
			MainApp.WriteComment(newptr2, comment2, displayUser);
		}
		MainApp.PrintPost(commentOnPost2);
	}
};

int main()
{
	SocialNetworkApp App;
	App.Run();


	system("pause");
	return 0;
}