#include <crow.h>
#include <pqxx/pqxx>
#include <nlohmann/json.hpp>
#include "dao/DAO_all.h"

using json = nlohmann::json;

int main() {
    // Database connection parameters
    std::string dbname = "mydb";
    std::string dbuser = "postgres";
    std::string dbpassword = "postgres";
    std::string dbhost = "127.0.0.1";
    std::string dbport = "5432";

    // Create connection string
    std::string conn_str = "dbname=" + dbname + " user=" + dbuser + " password=" + dbpassword +
                           " hostaddr=" + dbhost + " port=" + dbport;

    // Establish connection
    pqxx::connection conn(conn_str);

    UserDao userDao(conn);
    PortfolioDao portfolioDao(conn);
    PortfolioHoldingDao portfolioHoldingDao(conn);
    StockDao stockDao(conn);
    FriendshipDao friendshipDao(conn_str);
    StockListDao stockListDao(conn_str);
    StockListHoldingDao stockListHoldingDao(conn_str);
    ReviewDao reviewDao(conn_str);

    crow::SimpleApp app;

    // Variable to store the current logged-in user ID
    int current_logged_in_user_id = -1;

    // Signup route
    CROW_ROUTE(app, "/signup")
            .methods(crow::HTTPMethod::POST)
                    ([&userDao](const crow::request &req) {
                        auto body = nlohmann::json::parse(req.body);
                        std::string username = body["username"];
                        std::string password = body["password"];

                        // Check if user already exists
                        try {
                            User existingUser = userDao.getUserByUsername(username);
                            return crow::response(400, "User already exists");
                        } catch (const std::exception &) {
                            // User does not exist, proceed with creation
                            User newUser(1, username, password);
                            bool success = userDao.createUser(newUser);
                            if (success) {
                                return crow::response(201, "User created successfully");
                            } else {
                                return crow::response(500, "Failed to create user");
                            }
                        }
                    });

    // Login route
    CROW_ROUTE(app, "/login")
            .methods(crow::HTTPMethod::POST)
                    ([&userDao, &current_logged_in_user_id](const crow::request &req) {
                        if (current_logged_in_user_id != -1) {
                            return crow::response(400, "Another user is already logged in");
                        }

                        auto body = nlohmann::json::parse(req.body);
                        std::string username = body["username"];
                        std::string password = body["password"];

                        try {
                            User user = userDao.getUserByUsername(username);
                            if (user.getPassword() == password) {
                                current_logged_in_user_id = user.getId();
                                return crow::response(200, "Login successful");
                            } else {
                                return crow::response(401, "Invalid credentials");
                            }
                        } catch (const std::exception &) {
                            return crow::response(401, "Invalid credentials");
                        }
                    });

    // Logout route
    CROW_ROUTE(app, "/logout")
            .methods(crow::HTTPMethod::POST)
                    ([&current_logged_in_user_id](const crow::request &req) {
                        if (current_logged_in_user_id == -1) {
                            return crow::response(400, "No user is currently logged in");
                        }

                        current_logged_in_user_id = -1;
                        return crow::response(200, "Logout successful");
                    });

    // Get portfolios by user ID route
    CROW_ROUTE(app, "/users/<int>/portfolios")
            .methods(crow::HTTPMethod::GET)
                    ([&portfolioDao, &current_logged_in_user_id](const crow::request& req, int user_id) {
                        if (current_logged_in_user_id == -1) {
                            return crow::response(401, "No user is currently logged in");
                        }

                        if (current_logged_in_user_id != user_id) {
                            return crow::response(403, "Forbidden: You can only access your own portfolios");
                        }

                        try {
                            std::vector<Portfolio> portfolios = portfolioDao.getPortfoliosByUserId(user_id);
                            nlohmann::json response;
                            for (const auto& portfolio : portfolios) {
                                response.push_back({
                                                           {"id", portfolio.getId()},
                                                           {"name", portfolio.getName()},
                                                           {"cashAccount", portfolio.getCashAccount()}
                                                   });
                            }
                            return crow::response(response.dump());
                        } catch (const std::exception& e) {
                            std::cerr << e.what() << std::endl;
                            return crow::response(500, "Failed to retrieve portfolios");
                        }
                    });

    // Create a new portfolio
    CROW_ROUTE(app, "/users/<int>/portfolios")
            .methods(crow::HTTPMethod::POST)
                    ([&portfolioDao, &current_logged_in_user_id](const crow::request& req, int user_id) {
                        if (current_logged_in_user_id == -1) {
                            return crow::response(401, "No user is currently logged in");
                        }

                        if (current_logged_in_user_id != user_id) {
                            return crow::response(403, "Forbidden: You can only create portfolios for your own account");
                        }

                        try {
                            auto body = nlohmann::json::parse(req.body);
                            std::string name = body["name"];
                            double cashAccount = body["cashAccount"];

                            Portfolio newPortfolio(user_id, name, cashAccount);
                            portfolioDao.createPortfolio(newPortfolio);

                            return crow::response(201, "Portfolio created successfully");
                        } catch (const std::exception& e) {
                            std::cerr << e.what() << std::endl;
                            return crow::response(500, "Failed to create portfolio");
                        }
                    });

    // Update an existing portfolio
    CROW_ROUTE(app, "/users/<int>/portfolios/<int>")
            .methods(crow::HTTPMethod::PUT)
                    ([&portfolioDao, &current_logged_in_user_id](const crow::request& req, int user_id, int portfolio_id) {
                        if (current_logged_in_user_id == -1) {
                            return crow::response(401, "No user is currently logged in");
                        }

                        if (current_logged_in_user_id != user_id) {
                            return crow::response(403, "Forbidden: You can only update portfolios for your own account");
                        }

                        try {
                            Portfolio portfolio = portfolioDao.getPortfolioById(portfolio_id);

                            if (portfolio.getUserId() != user_id) {
                                return crow::response(403, "Forbidden: You can only update your own portfolios");
                            }

                            auto body = nlohmann::json::parse(req.body);
                            if (body.contains("name")) {
                                portfolio.setName(body["name"]);
                            }
                            if (body.contains("cashAccount")) {
                                portfolio.setCashAccount(body["cashAccount"]);
                            }

                            portfolioDao.updatePortfolio(portfolio);

                            return crow::response(200, "Portfolio updated successfully");
                        } catch (const std::exception& e) {
                            std::cerr << e.what() << std::endl;
                            return crow::response(500, "Failed to update portfolio");
                        }
                    });

    // Delete an existing portfolio
    CROW_ROUTE(app, "/users/<int>/portfolios/<int>")
            .methods(crow::HTTPMethod::DELETE)
                    ([&portfolioDao, &current_logged_in_user_id](const crow::request& req, int user_id, int portfolio_id) {
                        if (current_logged_in_user_id == -1) {
                            return crow::response(401, "No user is currently logged in");
                        }

                        if (current_logged_in_user_id != user_id) {
                            return crow::response(403, "Forbidden: You can only delete portfolios for your own account");
                        }

                        try {
                            Portfolio portfolio = portfolioDao.getPortfolioById(portfolio_id);

                            if (portfolio.getUserId() != user_id) {
                                return crow::response(403, "Forbidden: You can only delete your own portfolios");
                            }

                            portfolioDao.deletePortfolio(portfolio_id);

                            return crow::response(200, "Portfolio deleted successfully");
                        } catch (const std::exception& e) {
                            std::cerr << e.what() << std::endl;
                            return crow::response(500, "Failed to delete portfolio");
                        }
                    });

    // Get stock holdings by portfolio ID route
    CROW_ROUTE(app, "/portfolios/<int>")
            .methods(crow::HTTPMethod::GET)
                    ([&portfolioDao, &portfolioHoldingDao, &stockDao, &current_logged_in_user_id](const crow::request& req, int portfolio_id) {
                        if (current_logged_in_user_id == -1) {
                            return crow::response(401, "No user is currently logged in");
                        }

                        try {
                            Portfolio portfolio = portfolioDao.getPortfolioById(portfolio_id);

                            if (portfolio.getUserId() != current_logged_in_user_id) {
                                return crow::response(403, "Forbidden: You can only access your own portfolios");
                            }

                            std::vector<PortfolioHolding> holdings = portfolioHoldingDao.getPortfolioHoldingsByPortfolioId(portfolio_id);
                            nlohmann::json response;
                            for (const auto& holding : holdings) {
                                Stock stock = stockDao.getStockById(holding.getStockId());
                                response.push_back({
                                                           {"id", holding.getId()},
                                                           {"portfolioId", holding.getPortfolioId()},
                                                           {"stockId", holding.getStockId()},
                                                           {"quantity", holding.getQuantity()},
                                                           {"symbol", stock.getSymbol()},
                                                           {"currentPrice", stock.getCurrentPrice()}
                                                   });
                            }
                            return crow::response(response.dump());
                        } catch (const std::exception& e) {
                            std::cerr << e.what() << std::endl;
                            return crow::response(500, "Failed to retrieve portfolio holdings");
                        }
                    });

    // Add stock to portfolio (POST)
    CROW_ROUTE(app, "/portfolios/<int>/holding")
            .methods(crow::HTTPMethod::POST)
                    ([&portfolioDao, &portfolioHoldingDao, &stockDao, &current_logged_in_user_id](const crow::request& req, int portfolio_id) {
                        if (current_logged_in_user_id == -1) {
                            return crow::response(401, "No user is currently logged in");
                        }

                        try {
                            Portfolio portfolio = portfolioDao.getPortfolioById(portfolio_id);

                            if (portfolio.getUserId() != current_logged_in_user_id) {
                                return crow::response(403, "Forbidden: You can only access your own portfolios");
                            }

                            auto body = nlohmann::json::parse(req.body);
                            int stockId = body["stockId"];
                            double quantity = body["quantity"];

                            Stock stock = stockDao.getStockById(stockId);
                            double cost = stock.getCurrentPrice() * quantity;

                            if (portfolio.getCashAccount() < cost) {
                                return crow::response(400, "Insufficient funds");
                            }

                            portfolio.setCashAccount(portfolio.getCashAccount() - cost);
                            portfolioDao.updatePortfolio(portfolio);

                            PortfolioHolding newHolding(portfolio_id, stockId, quantity);
                            portfolioHoldingDao.addStockToPortfolio(newHolding);

                            nlohmann::json response;
                            response["cashAccount"] = portfolio.getCashAccount();
                            return crow::response(201, response.dump());
                        } catch (const std::exception& e) {
                            std::cerr << e.what() << std::endl;
                            return crow::response(500, "Failed to add stock to portfolio");
                        }
                    });

    // Update portfolio holding (PUT)
    CROW_ROUTE(app, "/portfolios/<int>/holding/<int>")
            .methods(crow::HTTPMethod::PUT)
                    ([&portfolioDao, &portfolioHoldingDao, &stockDao, &current_logged_in_user_id](const crow::request& req, int portfolio_id, int holding_id) {
                        if (current_logged_in_user_id == -1) {
                            return crow::response(401, "No user is currently logged in");
                        }

                        try {
                            Portfolio portfolio = portfolioDao.getPortfolioById(portfolio_id);

                            if (portfolio.getUserId() != current_logged_in_user_id) {
                                return crow::response(403, "Forbidden: You can only access your own portfolios");
                            }

                            PortfolioHolding holding = portfolioHoldingDao.getPortfolioHoldingById(holding_id);

                            if (holding.getPortfolioId() != portfolio_id) {
                                return crow::response(400, "Bad Request: Holding does not belong to the specified portfolio");
                            }

                            auto body = nlohmann::json::parse(req.body);
                            double newQuantity = body["quantity"];
                            double oldQuantity = holding.getQuantity();
                            Stock stock = stockDao.getStockById(holding.getStockId());
                            double priceDifference = stock.getCurrentPrice() * (newQuantity - oldQuantity);

                            if (portfolio.getCashAccount() < priceDifference) {
                                return crow::response(400, "Insufficient funds");
                            }

                            portfolio.setCashAccount(portfolio.getCashAccount() - priceDifference);
                            portfolioDao.updatePortfolio(portfolio);

                            holding.setQuantity(newQuantity);
                            portfolioHoldingDao.updatePortfolioHolding(holding);

                            nlohmann::json response;
                            response["cashAccount"] = portfolio.getCashAccount();
                            return crow::response(200, response.dump());
                        } catch (const std::exception& e) {
                            std::cerr << e.what() << std::endl;
                            return crow::response(500, "Failed to update portfolio holding");
                        }
                    });

    // Delete portfolio holding (DELETE)
    CROW_ROUTE(app, "/portfolios/<int>/holding/<int>")
            .methods(crow::HTTPMethod::DELETE)
                    ([&portfolioDao, &portfolioHoldingDao, &stockDao, &current_logged_in_user_id](const crow::request& req, int portfolio_id, int holding_id) {
                        if (current_logged_in_user_id == -1) {
                            return crow::response(401, "No user is currently logged in");
                        }

                        try {
                            Portfolio portfolio = portfolioDao.getPortfolioById(portfolio_id);

                            if (portfolio.getUserId() != current_logged_in_user_id) {
                                return crow::response(403, "Forbidden: You can only access your own portfolios");
                            }

                            PortfolioHolding holding = portfolioHoldingDao.getPortfolioHoldingById(holding_id);

                            if (holding.getPortfolioId() != portfolio_id) {
                                return crow::response(400, "Bad Request: Holding does not belong to the specified portfolio");
                            }

                            Stock stock = stockDao.getStockById(holding.getStockId());
                            double refund = stock.getCurrentPrice() * holding.getQuantity();

                            portfolio.setCashAccount(portfolio.getCashAccount() + refund);
                            portfolioDao.updatePortfolio(portfolio);

                            portfolioHoldingDao.deletePortfolioHolding(holding_id);

                            nlohmann::json response;
                            response["cashAccount"] = portfolio.getCashAccount();
                            return crow::response(200, response.dump());
                        } catch (const std::exception& e) {
                            std::cerr << e.what() << std::endl;
                            return crow::response(500, "Failed to delete portfolio holding");
                        }
                    });

    // Get friends for current user
    CROW_ROUTE(app, "/friends")
            .methods(crow::HTTPMethod::GET)
                    ([&friendshipDao, &current_logged_in_user_id](const crow::request& req) {
                        if (current_logged_in_user_id == -1) {
                            return crow::response(401, "No user is currently logged in");
                        }

                        try {
                            std::vector<Friendship> friendships = friendshipDao.getFriendshipsByUserId(current_logged_in_user_id);
                            nlohmann::json response;
                            for (const auto& friendship : friendships) {
                                response.push_back({
                                                           {"friendshipId", friendship.getFriendshipId()},
                                                           {"userId", friendship.getUserId()},
                                                           {"friendId", friendship.getFriendId()},
                                                           {"status", friendship.getStatus()},
                                                           {"requestDate", friendship.getRequestDate()},
                                                           {"responseDate", friendship.getResponseDate()}
                                                   });
                            }
                            return crow::response(response.dump());
                        } catch (const std::exception& e) {
                            std::cerr << e.what() << std::endl;
                            return crow::response(500, "Failed to retrieve friends");
                        }
                    });


    // Send friend request
    CROW_ROUTE(app, "/friends/request")
            .methods(crow::HTTPMethod::POST)
                    ([&friendshipDao, &current_logged_in_user_id](const crow::request& req) {
                        if (current_logged_in_user_id == -1) {
                            return crow::response(401, "No user is currently logged in");
                        }

                        try {
                            auto body = nlohmann::json::parse(req.body);
                            int friend_id = body["friendId"];

                            int friendship_id = friendshipDao.createFriendRequest(current_logged_in_user_id, friend_id);

                            nlohmann::json response;
                            response["friendshipId"] = friendship_id;
                            return crow::response(201, response.dump());
                        } catch (const std::exception& e) {
                            std::cerr << e.what() << std::endl;
                            return crow::response(500, "Failed to send friend request");
                        }
                    });

    // Accept friend request
    CROW_ROUTE(app, "/friends/accept/<int>")
            .methods(crow::HTTPMethod::PUT)
                    ([&friendshipDao, &current_logged_in_user_id](const crow::request& req, int friendship_id) {
                        if (current_logged_in_user_id == -1) {
                            return crow::response(401, "No user is currently logged in");
                        }

                        try {
                            Friendship friendship = friendshipDao.getFriendshipById(friendship_id);

                            if (friendship.getFriendId() != current_logged_in_user_id) {
                                return crow::response(403, "Forbidden: You can only accept friend requests sent to you");
                            }

                            friendshipDao.acceptFriendRequest(friendship_id);

                            return crow::response(200, "Friend request accepted");
                        } catch (const std::exception& e) {
                            std::cerr << e.what() << std::endl;
                            return crow::response(500, "Failed to accept friend request");
                        }
                    });

    // Delete friend
    CROW_ROUTE(app, "/friends/<int>")
            .methods(crow::HTTPMethod::DELETE)
                    ([&friendshipDao, &current_logged_in_user_id](const crow::request& req, int friendship_id) {
                        if (current_logged_in_user_id == -1) {
                            return crow::response(401, "No user is currently logged in");
                        }

                        try {
                            Friendship friendship = friendshipDao.getFriendshipById(friendship_id);

                            if (friendship.getUserId() != current_logged_in_user_id && friendship.getFriendId() != current_logged_in_user_id) {
                                return crow::response(403, "Forbidden: You can only delete your own friendships");
                            }

                            friendshipDao.deleteFriendship(friendship_id);

                            return crow::response(200, "Friend deleted");
                        } catch (const std::exception& e) {
                            std::cerr << e.what() << std::endl;
                            return crow::response(500, "Failed to delete friend");
                        }
                    });

    // Get stock lists for current user (GET)
    CROW_ROUTE(app, "/stocklists")
            .methods(crow::HTTPMethod::GET)
                    ([&stockListDao, &current_logged_in_user_id](const crow::request& req) {
                        if (current_logged_in_user_id == -1) {
                            return crow::response(401, "No user is currently logged in");
                        }

                        try {
                            std::vector<StockList> stockLists = stockListDao.getStockListsByUserId(current_logged_in_user_id);
                            nlohmann::json response;
                            for (const auto& stockList : stockLists) {
                                response.push_back({
                                                           {"listId", stockList.getListId()},
                                                           {"userId", stockList.getUserId()},
                                                           {"name", stockList.getName()},
                                                           {"isPublic", stockList.getIsPublic()}
                                                   });
                            }
                            return crow::response(response.dump());
                        } catch (const std::exception& e) {
                            std::cerr << e.what() << std::endl;
                            return crow::response(500, "Failed to retrieve stock lists");
                        }
                    });

    // Create a new stock list
    CROW_ROUTE(app, "/stocklists")
            .methods(crow::HTTPMethod::POST)
                    ([&stockListDao, &current_logged_in_user_id](const crow::request& req) {
                        if (current_logged_in_user_id == -1) {
                            return crow::response(401, "No user is currently logged in");
                        }

                        try {
                            auto body = nlohmann::json::parse(req.body);
                            std::string name = body["name"];
                            bool isPublic = body["isPublic"];

                            StockList stockList(current_logged_in_user_id, name, isPublic);
                            stockListDao.addStockList(stockList);

                            nlohmann::json response;
                            response["listId"] = stockList.getListId();
                            response["userId"] = stockList.getUserId();
                            response["name"] = stockList.getName();
                            response["isPublic"] = stockList.getIsPublic();
                            return crow::response(201, response.dump());
                        } catch (const std::exception& e) {
                            std::cerr << e.what() << std::endl;
                            return crow::response(500, "Failed to create stock list");
                        }
                    });

    // Update an existing stock list
    CROW_ROUTE(app, "/stocklists/<int>")
            .methods(crow::HTTPMethod::PUT)
                    ([&stockListDao, &current_logged_in_user_id](const crow::request& req, int listId) {
                        if (current_logged_in_user_id == -1) {
                            return crow::response(401, "No user is currently logged in");
                        }

                        try {
                            auto body = nlohmann::json::parse(req.body);
                            std::string name = body["name"];
                            bool isPublic = body["isPublic"];

                            StockList stockList = stockListDao.getStockListById(listId);

                            if (stockList.getUserId() != current_logged_in_user_id) {
                                return crow::response(403, "Forbidden: You can only update your own stock lists");
                            }

                            stockList.setName(name);
                            stockList.setIsPublic(isPublic);
                            stockListDao.updateStockList(stockList);

                            nlohmann::json response;
                            response["listId"] = stockList.getListId();
                            response["userId"] = stockList.getUserId();
                            response["name"] = stockList.getName();
                            response["isPublic"] = stockList.getIsPublic();
                            return crow::response(200, response.dump());
                        } catch (const std::exception& e) {
                            std::cerr << e.what() << std::endl;
                            return crow::response(500, "Failed to update stock list");
                        }
                    });

    // Delete an existing stock list
    CROW_ROUTE(app, "/stocklists/<int>")
            .methods(crow::HTTPMethod::DELETE)
                    ([&stockListDao, &current_logged_in_user_id](const crow::request& req, int listId) {
                        if (current_logged_in_user_id == -1) {
                            return crow::response(401, "No user is currently logged in");
                        }

                        try {
                            StockList stockList = stockListDao.getStockListById(listId);

                            if (stockList.getUserId() != current_logged_in_user_id) {
                                return crow::response(403, "Forbidden: You can only delete your own stock lists");
                            }

                            stockListDao.deleteStockList(listId);

                            return crow::response(200, "Stock list deleted");
                        } catch (const std::exception& e) {
                            std::cerr << e.what() << std::endl;
                            return crow::response(500, "Failed to delete stock list");
                        }
                    });

    // Get stock list holdings for a specific stock list
    CROW_ROUTE(app, "/stocklists/<int>/holding")
            .methods(crow::HTTPMethod::GET)
                    ([&stockListHoldingDao, &stockDao, &stockListDao, &current_logged_in_user_id](const crow::request& req, int listId) {
                        if (current_logged_in_user_id == -1) {
                            return crow::response(401, "No user is currently logged in");
                        }

                        try {
                            StockList stockList = stockListDao.getStockListById(listId);
                            if (stockList.getUserId() != current_logged_in_user_id) {
                                return crow::response(403, "Forbidden: You can only view holdings of your own stock lists");
                            }

                            std::vector<StockListHolding> holdings = stockListHoldingDao.getStockListHoldingsByListId(listId);
                            nlohmann::json response;
                            for (const auto& holding : holdings) {
                                Stock stock = stockDao.getStockById(holding.getStockId());
                                nlohmann::json holdingJson;
                                holdingJson["listHoldingId"] = holding.getListHoldingId();
                                holdingJson["listId"] = holding.getListId();
                                holdingJson["stockId"] = holding.getStockId();
                                holdingJson["shares"] = holding.getShares();
                                holdingJson["stockSymbol"] = stock.getSymbol();
                                holdingJson["currentPrice"] = stock.getCurrentPrice();
                                response.push_back(holdingJson);
                            }
                            return crow::response(response.dump());
                        } catch (const std::exception& e) {
                            std::cerr << e.what() << std::endl;
                            return crow::response(500, "Failed to retrieve stock list holdings");
                        }
                    });

    // Create a new stock list holding
    CROW_ROUTE(app, "/stocklists/<int>/holding")
            .methods(crow::HTTPMethod::POST)
                    ([&stockListHoldingDao, &stockListDao, &current_logged_in_user_id](const crow::request& req, int listId) {
                        if (current_logged_in_user_id == -1) {
                            return crow::response(401, "No user is currently logged in");
                        }

                        try {
                            StockList stockList = stockListDao.getStockListById(listId);
                            if (stockList.getUserId() != current_logged_in_user_id) {
                                return crow::response(403, "Forbidden: You can only add holdings to your own stock lists");
                            }

                            auto body = nlohmann::json::parse(req.body);
                            int stockId = body["stockId"];
                            int shares = body["shares"];

                            StockListHolding stockListHolding(listId, stockId, shares);
                            int stockListHoldingId = stockListHoldingDao.addStockListHolding(stockListHolding);

                            nlohmann::json response;
                            response["listHoldingId"] = stockListHoldingId;
                            response["listId"] = stockListHolding.getListId();
                            response["stockId"] = stockListHolding.getStockId();
                            response["shares"] = stockListHolding.getShares();
                            return crow::response(201, response.dump());
                        } catch (const std::exception& e) {
                            std::cerr << e.what() << std::endl;
                            return crow::response(500, "Failed to create stock list holding");
                        }
                    });

    // Update an existing stock list holding
    CROW_ROUTE(app, "/stocklists/<int>/holding/<int>")
            .methods(crow::HTTPMethod::PUT)
                    ([&stockListHoldingDao, &stockListDao, &current_logged_in_user_id](const crow::request& req, int listId, int holdingId) {
                        if (current_logged_in_user_id == -1) {
                            return crow::response(401, "No user is currently logged in");
                        }

                        try {
                            StockList stockList = stockListDao.getStockListById(listId);
                            if (stockList.getUserId() != current_logged_in_user_id) {
                                return crow::response(403, "Forbidden: You can only update holdings in your own stock lists");
                            }

                            auto body = nlohmann::json::parse(req.body);
                            int shares = body["shares"];

                            StockListHolding holding = stockListHoldingDao.getStockListHoldingById(holdingId);
                            if (holding.getListId() != listId) {
                                return crow::response(400, "Invalid holding ID for the specified stock list");
                            }

                            holding.setShares(shares);
                            stockListHoldingDao.updateStockListHolding(holding);

                            nlohmann::json response;
                            response["listHoldingId"] = holding.getListHoldingId();
                            response["listId"] = holding.getListId();
                            response["stockId"] = holding.getStockId();
                            response["shares"] = holding.getShares();
                            return crow::response(200, response.dump());
                        } catch (const std::exception& e) {
                            std::cerr << e.what() << std::endl;
                            return crow::response(500, "Failed to update stock list holding");
                        }
                    });

    // Delete an existing stock list holding
    CROW_ROUTE(app, "/stocklists/<int>/holding/<int>")
            .methods(crow::HTTPMethod::DELETE)
                    ([&stockListHoldingDao, &stockListDao, &current_logged_in_user_id](const crow::request& req, int listId, int holdingId) {
                        if (current_logged_in_user_id == -1) {
                            return crow::response(401, "No user is currently logged in");
                        }

                        try {
                            StockList stockList = stockListDao.getStockListById(listId);
                            if (stockList.getUserId() != current_logged_in_user_id) {
                                return crow::response(403, "Forbidden: You can only delete holdings from your own stock lists");
                            }

                            StockListHolding holding = stockListHoldingDao.getStockListHoldingById(holdingId);
                            if (holding.getListId() != listId) {
                                return crow::response(400, "Invalid holding ID for the specified stock list");
                            }

                            stockListHoldingDao.deleteStockListHolding(holdingId);

                            return crow::response(200, "Stock list holding deleted");
                        } catch (const std::exception& e) {
                            std::cerr << e.what() << std::endl;
                            return crow::response(500, "Failed to delete stock list holding");
                        }
                    });

    // Get reviews for a specific stock list
    CROW_ROUTE(app, "/stocklists/<int>/reviews")
            .methods(crow::HTTPMethod::GET)
                    ([&reviewDao, &stockListDao, &current_logged_in_user_id](const crow::request& req, int listId) {
                        if (current_logged_in_user_id == -1) {
                            return crow::response(401, "No user is currently logged in");
                        }

                        try {
                            StockList stockList = stockListDao.getStockListById(listId);
                            if (stockList.getUserId() != current_logged_in_user_id) {
                                return crow::response(403, "Forbidden: You can only view reviews of your own stock lists");
                            }

                            std::vector<Review> reviews = reviewDao.getReviewsByListId(listId);
                            nlohmann::json response;
                            for (const auto& review : reviews) {
                                nlohmann::json reviewJson;
                                reviewJson["reviewId"] = review.getReviewId();
                                reviewJson["listId"] = review.getListId();
                                reviewJson["userId"] = review.getUserId();
                                reviewJson["content"] = review.getContent();
                                reviewJson["reviewDate"] = std::to_string(review.getReviewDate());
                                response.push_back(reviewJson);
                            }
                            return crow::response(response.dump());
                        } catch (const std::exception& e) {
                            std::cerr << e.what() << std::endl;
                            return crow::response(500, "Failed to retrieve reviews");
                        }
                    });

    // Add a new review for a specific stock list
    CROW_ROUTE(app, "/stocklists/<int>/reviews")
            .methods(crow::HTTPMethod::POST)
                    ([&reviewDao, &stockListDao, &current_logged_in_user_id](const crow::request& req, int listId) {
                        if (current_logged_in_user_id == -1) {
                            return crow::response(401, "No user is currently logged in");
                        }

                        try {
                            StockList stockList = stockListDao.getStockListById(listId);
                            if (stockList.getUserId() != current_logged_in_user_id) {
                                return crow::response(403, "Forbidden: You can only add reviews to your own stock lists");
                            }

                            auto body = nlohmann::json::parse(req.body);
                            std::string content = body["content"];
                            std::time_t reviewDate = std::time(nullptr);  // Current time

                            Review review(listId, current_logged_in_user_id, content, reviewDate);
                            int reviewId = reviewDao.addReview(review);

                            nlohmann::json response;
                            response["reviewId"] = reviewId;
                            response["listId"] = review.getListId();
                            response["userId"] = review.getUserId();
                            response["content"] = review.getContent();
                            response["reviewDate"] = std::to_string(review.getReviewDate());
                            return crow::response(201, response.dump());
                        } catch (const std::exception& e) {
                            std::cerr << e.what() << std::endl;
                            return crow::response(500, "Failed to add review");
                        }
                    });

    // Update an existing review for a specific stock list
    CROW_ROUTE(app, "/stocklists/<int>/reviews/<int>")
            .methods(crow::HTTPMethod::PUT)
                    ([&reviewDao, &stockListDao, &current_logged_in_user_id](const crow::request& req, int listId, int reviewId) {
                        if (current_logged_in_user_id == -1) {
                            return crow::response(401, "No user is currently logged in");
                        }

                        try {
                            StockList stockList = stockListDao.getStockListById(listId);
                            if (stockList.getUserId() != current_logged_in_user_id) {
                                return crow::response(403, "Forbidden: You can only update reviews of your own stock lists");
                            }

                            Review review = reviewDao.getReviewById(reviewId);
                            if (review.getListId() != listId) {
                                return crow::response(400, "Invalid review ID for the specified stock list");
                            }

                            auto body = nlohmann::json::parse(req.body);
                            std::string content = body["content"];
                            std::time_t reviewDate = std::time(nullptr);  // Current time

                            review.setContent(content);
                            review.setReviewDate(reviewDate);
                            reviewDao.updateReview(review);

                            nlohmann::json response;
                            response["reviewId"] = review.getReviewId();
                            response["listId"] = review.getListId();
                            response["userId"] = review.getUserId();
                            response["content"] = review.getContent();
                            response["reviewDate"] = std::to_string(review.getReviewDate());
                            return crow::response(200, response.dump());
                        } catch (const std::exception& e) {
                            std::cerr << e.what() << std::endl;
                            return crow::response(500, "Failed to update review");
                        }
                    });

    // Delete an existing review for a specific stock list
    CROW_ROUTE(app, "/stocklists/<int>/reviews/<int>")
            .methods(crow::HTTPMethod::DELETE)
                    ([&reviewDao, &stockListDao, &current_logged_in_user_id](const crow::request& req, int listId, int reviewId) {
                        if (current_logged_in_user_id == -1) {
                            return crow::response(401, "No user is currently logged in");
                        }

                        try {
                            StockList stockList = stockListDao.getStockListById(listId);
                            if (stockList.getUserId() != current_logged_in_user_id) {
                                return crow::response(403, "Forbidden: You can only delete reviews from your own stock lists");
                            }

                            Review review = reviewDao.getReviewById(reviewId);
                            if (review.getListId() != listId) {
                                return crow::response(400, "Invalid review ID for the specified stock list");
                            }

                            reviewDao.deleteReview(reviewId);

                            return crow::response(200, "Review deleted");
                        } catch (const std::exception& e) {
                            std::cerr << e.what() << std::endl;
                            return crow::response(500, "Failed to delete review");
                        }
                    });

    app.port(8080).multithreaded().run();
}

