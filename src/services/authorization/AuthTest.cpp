#include <iostream>
#include "AuthorizationService.h"

int main(void){
    User* u = new User();
    AuthorizationService* authService = new AuthorizationService(u);
    cout << authService->hasAccessTo("");
    return 0;
}