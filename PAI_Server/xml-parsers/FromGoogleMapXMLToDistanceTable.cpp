#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "FromGoogleMapXMLToDistanceTable.h"

// Constructeur
FromGoogleMapXMLToDistanceTable::FromGoogleMapXMLToDistanceTable() : LwSaxParser()
{
    state = UNKNOWN;
    adresseCourante = "";
	numeroLigne = -1;
}


FromGoogleMapXMLToDistanceTable::~FromGoogleMapXMLToDistanceTable()
{
    
}


std::vector<std::string> FromGoogleMapXMLToDistanceTable::getAdresses()
{
    
    return adresses;
}

std::vector< std::vector<double> > FromGoogleMapXMLToDistanceTable::getDistances()
{
    
    return distanceMatrix;
    
}

void FromGoogleMapXMLToDistanceTable::on_start_document()
{
	state = START;
}

void FromGoogleMapXMLToDistanceTable::on_end_document()
{
	state = END;
}

void FromGoogleMapXMLToDistanceTable::on_start_element(const Glib::ustring& name, const AttributeList& attributes)
{
	
	if(name == "origin_address")
	{
		state = ORIGIN_ADDRESS;
	}
	else if(name == "value")
	{
		if(state == DISTANCE)
			state = VALUE;
	}
	else if(name == "distance")
	{
		state = DISTANCE;
	}
}

void FromGoogleMapXMLToDistanceTable::on_end_element(const Glib::ustring& name)
{
	
	switch(state)
	{
		case ORIGIN_ADDRESS:
			adresses.push_back(adresseCourante);
			adresseCourante = "";
			state = UNKNOWN;
			break;
		case VALUE:
			state = DISTANCE;
			break;
		case DISTANCE:
			state = ELEMENT;
			break;
		default:
			break;
	}
	
	if(name == "row")
	{
		distanceMatrix.push_back(ligne);
		ligne.clear();
	}
}

void FromGoogleMapXMLToDistanceTable::on_characters(const Glib::ustring& text)
{    
    switch(state)
    {
		case ORIGIN_ADDRESS:
			adresseCourante += text.c_str();
			break;
		case VALUE:
			ligne.push_back(atof(text.c_str()));
			break;
		default:
			break;
	}
}


void FromGoogleMapXMLToDistanceTable::on_warning(const Glib::ustring& text)
{
	std::cout << "on_warning(): " << text.c_str() << std::endl;
}

void FromGoogleMapXMLToDistanceTable::on_error(const Glib::ustring& text)
{
	std::cout << "on_error(): " << text.c_str() << std::endl;
}

void FromGoogleMapXMLToDistanceTable::on_fatal_error(const Glib::ustring& text)
{
	std::cout << "on_fatal_error(): " << text.c_str() << std::endl;
}

