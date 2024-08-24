#include <sqlite3.h>
#include <stdlib.h>
#include <iostream>
#include <iomanip>

#define DATABASE "fruit.db"
typedef int (*task)(int);
sqlite3 *db;

//创建数据库，并检查是否存在
void create_database(void)
{
  // 尝试以读写模式打开数据库，并且创建数据库文件（如果它不存在）
    int ret = sqlite3_open_v2(DATABASE, &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);

    if(ret != SQLITE_OK)
    {
        std::cout << "Failed to open database: " << sqlite3_errmsg(db) << std::endl;
        exit(EXIT_FAILURE);
    }
    
    // 如果数据库文件已经存在，sqlite3_open_v2 将成功打开它并返回 SQLITE_OK
    std::cout << "Database opened successfully" << std::endl; 
}
//关闭数据库
void close_database(void)
{
    int ret = sqlite3_close(db);
    if(ret != SQLITE_OK)
    {
       std::cout << sqlite3_errmsg(db) << std::endl;
    }
    else
    {
        std::cout << "close success" << std::endl;
		exit(0);
    }
}
//创建表并检查是否存在
void create_table(void)
{
    // 检查表是否存在
    const char* check_table_sql = "SELECT name FROM sqlite_master WHERE type='table' AND name='fruit';";
    sqlite3_stmt* stmt;
    int ret = sqlite3_prepare_v2(db, check_table_sql, -1, &stmt, NULL);

    if (ret != SQLITE_OK) {
        std::cout << "Failed to check if table exists: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        exit(EXIT_FAILURE);
    }

    bool table_exists = false;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        table_exists = true; // Table exists
    }

    sqlite3_finalize(stmt);

    if (!table_exists) {
        // 表不存在，创建表
        const char* create_table_sql = "CREATE TABLE fruit (id INTEGER PRIMARY KEY AUTOINCREMENT, name CHAR, price INTEGER, num INTEGER, store_time TEXT, ship_out_time TEXT)";
        char *errmsg = NULL;
        ret = sqlite3_exec(db, create_table_sql, NULL, NULL, &errmsg);

        if (ret != SQLITE_OK) {
            std::cout << "Failed to create table: " << errmsg << std::endl;
            sqlite3_free(errmsg);
            sqlite3_close(db);
            exit(EXIT_FAILURE);
        }

        std::cout << "Table created successfully" << std::endl;
    } else {
        std::cout << "Table already exists" << std::endl;
    }
}
//更改表结构
void modify_table_structure(void)
{
    std::cout<<"------------------------------------------------------------------"<< std::endl;
    std::cout << "(0) Add Column"<<std::endl;
    std::cout << "(1) Rename Column" << std::endl;
    std::cout << "(2) Delete Column" << std::endl;
    std::cout << "(3) Modify the column type" << std::endl;
    std::cout << "(4) Rename Table" << std::endl;
}
void insert() {  
    std::string goods;
    std::cout << "Choose a goods: ";
    std::cin >> goods;

    // 构建查询语句，检查水果是否存在
    std::string check_query = "SELECT num FROM fruit WHERE name = '" + goods + "';";
    sqlite3_stmt* stmt;
    int ret = sqlite3_prepare_v2(db, check_query.c_str(), -1, &stmt, 0);

    if (ret != SQLITE_OK) {
        std::cout << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        exit(EXIT_FAILURE);
    }

    ret = sqlite3_step(stmt);
    if (ret == SQLITE_ROW) {
        // 水果存在，获取数量并增加数量
        int current_num = sqlite3_column_int(stmt, 0);
        int added_num;
        std::cout << "Please input the number of " << goods << " to add: ";
        std::cin >> added_num;
        current_num += added_num;

        std::string update_query = "UPDATE fruit SET num = " + std::to_string(current_num) + " WHERE name = '" + goods + "';";
        char* errmsg = NULL;
        ret = sqlite3_exec(db, update_query.c_str(), NULL, NULL, &errmsg);

        if (ret != SQLITE_OK) {
            std::cout << "Failed to update table: " << errmsg << std::endl;
            sqlite3_free(errmsg);
        } else {
            std::cout << "Updated the number of " << goods << " to " << current_num << "." << std::endl;
        }
    } else {
        // 水果不存在，执行插入操作
        int price;
        int quantity;
        std::cout << "Please input the price of " << goods << ": ";
        std::cin >> price;
        std::cout << "Please input the number of " << goods << ": ";
        std::cin >> quantity;

        // 获取当前时间作为store_time
        time_t now = time(0);
        char time_str[20];
        strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", localtime(&now));

        std::string insert_query = "INSERT INTO fruit (name, price, num, store_time) VALUES ('" + goods + "', " + std::to_string(price) + ", " + std::to_string(quantity) + ", '" + time_str + "');";
        char* errmsg = NULL;
        ret = sqlite3_exec(db, insert_query.c_str(), NULL, NULL, &errmsg);

        if (ret != SQLITE_OK) {
            std::cout << "Failed to insert into table: " << errmsg << std::endl;
            sqlite3_free(errmsg);
        } else {
            std::cout << "Inserted " << goods << " with quantity " << quantity << " and store time " << time_str << "." << std::endl;
        }
    }

    sqlite3_finalize(stmt);
}

void delete_fruit() {
    std::string goods;
    std::cout << "Enter the name of the fruit to delete: ";
    std::cin >> goods;

    // 构建查询语句，检查水果是否存在
    std::string check_query = "SELECT num FROM fruit WHERE name = '" + goods + "';";
    sqlite3_stmt* stmt;
    int ret = sqlite3_prepare_v2(db, check_query.c_str(), -1, &stmt, 0);

    if (ret != SQLITE_OK) {
        std::cout << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        exit(EXIT_FAILURE);
    }

    ret = sqlite3_step(stmt);
    if (ret == SQLITE_ROW) {
        // 水果存在，获取当前数量
        int current_num = sqlite3_column_int(stmt, 0);
        std::cout << "Current quantity of " << goods << " is: " << current_num << std::endl;

        int reduce_num;
        while (true) {
            std::cout << "Enter the quantity to remove: ";
            std::cin >> reduce_num;

            if (reduce_num <= current_num) {
                break;
            } else {
                std::cout << "The quantity to remove exceeds the available quantity. Please try again." << std::endl;
            }
        }

        // 计算新的数量并获取当前时间作为 ship_out_time
        int new_quantity = current_num - reduce_num;
        time_t now = time(0);
        char time_str[20];
        strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", localtime(&now));

        // 构建更新语句
        std::string update_query = "UPDATE fruit SET num = " + std::to_string(new_quantity) + ", ship_out_time = '" + time_str + "' WHERE name = '" + goods + "';";
        char* errmsg = NULL;
        ret = sqlite3_exec(db, update_query.c_str(), NULL, NULL, &errmsg);

        if (ret != SQLITE_OK) {
            std::cout << "Failed to update table: " << errmsg << std::endl;
            sqlite3_free(errmsg);
        } else {
            std::cout << "Reduced " << reduce_num << " " << goods << ". New quantity is " << new_quantity << ". Ship out time updated to " << time_str << "." << std::endl;
        }
    } else {
        // 水果不存在，提示并退出
        std::cout << "Fruit not found in the database." << std::endl;
    }

    sqlite3_finalize(stmt);
}

void modify() {
    std::string goods;
    std::cout << "Enter the name of the fruit to modify: ";
    std::cin >> goods;

    // 构建查询语句，检查水果是否存在
    std::string check_query = "SELECT price FROM fruit WHERE name = '" + goods + "';";
    sqlite3_stmt* stmt;
    int ret = sqlite3_prepare_v2(db, check_query.c_str(), -1, &stmt, 0);

    if (ret != SQLITE_OK) {
        std::cout << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        exit(EXIT_FAILURE);
    }

    ret = sqlite3_step(stmt);
    if (ret == SQLITE_ROW) {
        // 水果存在，获取当前价格
        int current_price = sqlite3_column_int(stmt, 0);
        std::cout << "Current price of " << goods << " is: " << current_price << std::endl;

        int new_price;
        std::cout << "Enter the new price: ";
        std::cin >> new_price;

        // 构建更新语句
        std::string update_query = "UPDATE fruit SET price = " + std::to_string(new_price) + " WHERE name = '" + goods + "';";
        char* errmsg = NULL;
        ret = sqlite3_exec(db, update_query.c_str(), NULL, NULL, &errmsg);

        if (ret != SQLITE_OK) {
            std::cout << "Failed to update price: " << errmsg << std::endl;
            sqlite3_free(errmsg);
        } else {
            std::cout << "Price of " << goods << " has been updated to " << new_price << "." << std::endl;
        }
    } else {
        // 水果不存在，提示并退出
        std::cout << "Fruit not found in the database." << std::endl;
    }

    sqlite3_finalize(stmt);
}
void display_fruit(void)
{
    int option;
    std::cout << "Choose an option:\n";
    std::cout << "(1) Display a specific fruit\n";
    std::cout << "(2) Display the entire table\n";
    std::cout << "Enter your choice: ";
    std::cin >> option;

    std::string query;

    if (option == 1) {
        // 查询特定水果
        std::string goods;
        std::cout << "Enter the name of the fruit to display: ";
        std::cin >> goods;
        query = "SELECT id, name, price, num, store_time, ship_out_time FROM fruit WHERE name = '" + goods + "';";
    } else if (option == 2) {
        // 查询整个表
        query = "SELECT id, name, price, num, store_time, ship_out_time FROM fruit;";
    } else {
        std::cout << "Invalid option." << std::endl;
        return;
    }

    sqlite3_stmt* stmt;
    int ret = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, 0);

    if (ret != SQLITE_OK) {
        std::cout << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        exit(EXIT_FAILURE);
    }

    std::cout << std::left << std::setw(15) << "ID"
              << std::setw(15) << "Name"
              << std::setw(15) << "Price"
              << std::setw(15) << "Quantity"
              << std::setw(25) << "Store Time"
              << std::setw(25) << "Ship Out Time" << std::endl;

    while ((ret = sqlite3_step(stmt)) == SQLITE_ROW) {
        // 获取并显示字段值
        int id = sqlite3_column_int(stmt, 0);
        const unsigned char* name = sqlite3_column_text(stmt, 1);
        int price = sqlite3_column_int(stmt, 2);
        int num = sqlite3_column_int(stmt, 3);

        // 检查 store_time 和 ship_out_time 是否为 NULL
        const unsigned char* store_time = sqlite3_column_text(stmt, 4);
        const unsigned char* ship_out_time = sqlite3_column_text(stmt, 5);

        std::cout << std::left << std::setw(15) << id
                  << std::setw(15) << name
                  << std::setw(15) << price
                  << std::setw(15) << num
                  << std::setw(25) << (store_time ? reinterpret_cast<const char*>(store_time) : "NULL")
                  << std::setw(25) << (ship_out_time ? reinterpret_cast<const char*>(ship_out_time) : "NULL")
                  << std::endl;
    }

    if (ret != SQLITE_DONE) {
        std::cout << "Failed to retrieve data: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
}
//更新表
void updata_table(void)
{
   //insert one good
    char choice;
    while(true)
    {
        std::cout<<"------------------------------------------------------------------"<< std::endl;
        std::cout << "(0) exit"<<std::endl;
        std::cout << "(1) Insert data" << std::endl;
        std::cout << "(2) Delete data" << std::endl;
        std::cout << "(3) Modify price" << std::endl;
        std::cout << "(4) display_fruit" << std::endl;
        std::cout << "please input your choice: ";
        std::cin >> choice;
        switch (choice)
        {
            case '0':
                close_database();
                break;
            case '1':
                insert();
                break;
            case '2':
                delete_fruit();
                break;
            case '3':
                modify();
                break;
            case '4':
                display_fruit();
                break;
            default:
                std::cout << "Invalid choice,please input again."<<std::endl;
        }
    }
    //delete one good

    //Modify price

}
//具体页面显示及细节操作
int view(int view_task) {
    if (view_task == 1) {
        std::cout<<"------------------------------------------------------------------"<< std::endl;
        std::cout << "(0) exit\n(1) create a database" << std::endl;
        
        char choice;
		while(true)//输入错误后重新输入
		{
			std::cout << "please input your choice: ";
			std::cin >> choice;
			switch (choice) {
				case '0':
					close_database();
					return 0;
				case '1':
					create_database();
					return 2;
				default:
					std::cout << "Invalid choice, please input again." << std::endl;
			}
		}
    }

    if (view_task == 2) 
    {
        std::cout<<"------------------------------------------------------------------"<< std::endl;
        std::cout << "(0) exit \n(1) create an init table" << std::endl;

        char choice = '0';
		while(true)//输入错误后重新输入
		{        
			std::cout << "please input your choice: ";
			std::cin >> choice;

			switch (choice) {
				case '0':
					close_database();
					return 0;
				case '1':
					create_table();
					return 3;
				default:
					std::cout << "Invalid choice, please input again." << std::endl;
			}
		}
	}
    if(view_task == 3)
    {
        std::cout<<"------------------------------------------------------------------"<< std::endl;
        std::cout << "Whether to modify the table structure" << std::endl;
        std::cout << "(0) exit \n(1) yes \n(2) no" << std::endl;

        char choice = '0';
        while(true)//输入错误后重新输入
        {
            std::cout << "please input your choice: ";
            std::cin >> choice;

            switch(choice)
            {
                case '0':
                    close_database();
                    return 0;//以防万一
                case '1':
                    modify_table_structure();
                    return 4;
                case '2':
                    return 4;
                default:
                    std::cout << "Invalid choice, please input again." << std::endl;
            }
        }
    }


	if (view_task == 4) {
        std::cout<<"------------------------------------------------------------------"<< std::endl;
		std::cout << "(0) exit\n(1) update table" << std::endl;
        
		char choice;
		while(true)//输入错误后重新输入
        {
			std::cout << "please input your choice: ";
			std::cin >> choice;
			switch (choice) {
				case '0':
					close_database();
					return 0;
				case '1':
					updata_table();
					return 4;
				default:
					std::cout << "Invalid choice, please input again." << std::endl;
			}
		}
	}
    return 0; // 默认返回值，以防万一
}
//工作流程
void working(void)
{
	task operation = view;
    int ret = 1;
    
    switch(ret)
    {
        case 1:
            ret = operation(ret);//创建数据库
        case 2:
            ret = operation(ret);//创建表
            break;
        case 3:
            ret = operation(ret);//修改表结构
        default:
            close_database();//退出
            break;		
    }
     do 
     {
         ret = operation(ret);//更新表
     }while(ret==4);
}

int main (int argc, char *argv[])
{  
    working();
    return 0;
}
