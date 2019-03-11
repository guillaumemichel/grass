#include <iostream>
#include "AuthorizationService.h"

int main(void){
    User* u = new User();
    AuthorizationService* authService = new AuthorizationService(u);
    cout << authService->hasAccessTo("");
    delete authService;
    delete u;
    return 0;
}