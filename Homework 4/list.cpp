void listAll(const MenuItem* m, string path) // two-parameter overload
{
	if (m->name() != "") {
		if(path.length() == 0)
			path += m->name();
		else
			path += '/'+m->name();
		cout << path << endl;
	}
	if (m->menuItems() == NULL)
		return;
	for (int i = 0; i < m->menuItems()->size(); i++) {
		listAll((*(m->menuItems()))[i], path);
	}
}

