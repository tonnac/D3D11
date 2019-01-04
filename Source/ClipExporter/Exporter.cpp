#include "Exporter.h"


Exporter::Exporter()
{
}


Exporter::~Exporter()
{
}


int Exporter::ExtCount()
{
	return 1;
}

const MCHAR *Exporter::Ext(int )
{
	return L"CLP";
}

const MCHAR *Exporter::LongDesc()
{
	return L"ClipExporter V1.0";
}

const MCHAR *Exporter::ShortDesc()
{
	return L"ClipExporter";
}

const MCHAR *Exporter::AuthorName()
{
	return L"ddda";
}

const MCHAR *Exporter::CopyrightMessage()
{
	return L"";
}

const MCHAR *Exporter::OtherMessage1()
{
	return L"";
}

const MCHAR *Exporter::OtherMessage2()
{
	return L"";
}

unsigned int Exporter::Version()
{
	return 100;
}

void Exporter::ShowAbout(HWND)
{
}

BOOL Exporter::SupportsOptions(int, DWORD )
{
	return TRUE;
}


int	Exporter::DoExport(const MCHAR *name, ExpInterface *ei, Interface *i,
	BOOL suppressPrompts, DWORD options)
{
	ClipExporter exporter;
	
	if (!exporter.Initialize(i, name, LongDesc(), true))
	{
		return FALSE;
	}
	
	return exporter.Run();
}