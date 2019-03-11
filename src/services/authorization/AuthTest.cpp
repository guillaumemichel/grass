#include <iostream>
#include "AuthorizationService.h"

int main(void){
    User* u = new User();
    u->setAuthenticated(true);
    AuthorizationService* authService = new AuthorizationService(u);
    cout << authService->hasAccessTo("grep");
    delete authService;
    delete u;
    return 0;
}