/*
 *	Copyright 2012 Potion Design. All rights reserved.
 *	This file is part of pocode.
 *
 *	pocode is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU Lesser General Public License as 
 *	published by the Free Software Foundation, either version 3 of 
 *	the License, or (at your option) any later version.
 *
 *	pocode is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU Lesser General Public License for more details.
 *
 *	You should have received a copy of the GNU Lesser General Public 
 *	License along with pocode.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#pragma once


/*	Created by Stephen Varga on 9/15/11.
 *	Copyright 2011 Potion. All rights reserved.
 */

#include "poDictionary.h"

#include <string>
#include <vector>

#define PO_COL_TYPE_INT     1
#define PO_COL_TYPE_FLOAT   2
#define PO_COL_TYPE_STRING  3

struct sqlite3;

//------------------------------------------------------------
//poSqlite3Column
typedef struct {
    std::string name;
    int type;
} poSqlite3Column;

//------------------------------------------------------------
//poSqlite3Result
class poSqlite3Result {
    public:
        poSqlite3Result(std::string query, bool bVerbose=false);
        ~poSqlite3Result();
        
        std::string query;
        
        std::vector <poSqlite3Column> columns;
        std::string getColumnNames();
        
        int getNumRows();
        poDictionary getRow(int rowNum);
        
        std::vector <poDictionary> rows;

        std::string errorMessage;
    private:
        bool bVerbose;
        int nRows;
};

class poSqlite3 {
    public:
        poSqlite3();
        poSqlite3(std::string url, bool bVerbose=false);
        virtual ~poSqlite3();
        
        void setVerbose(bool isVerbose);
        bool openDatabase(std::string url, bool bOverwrite = false);
        
        //Begin/End transaction helps batch execute many calls
        //Great for speed improvements
        void beginTransaction();
        void endTransaction();
        
        poSqlite3Result query(std::string query);
        
        std::string escapeQuotes(std::string text);
        
        void save();
        void save(const char* url);
        
        void close();
        
        sqlite3 *db;
    private:
        bool bLoaded;
        bool bVerbose;
};

