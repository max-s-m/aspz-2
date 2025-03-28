//якби ця програма виконувалась за заміни IP на вершину стека 
#include <stdio.h>

void func() {
    printf("Inside func\n");
}

int main() {
    printf("Before func\n");
 
    //виклик func за нормальних обставин: поточний IP (адреса інструкції після виклику func) на стеку як адреса повернення,
    //IP змінюється і вказує на першу інструкцію у func(), завершується func(), адреса повернення переходить зі стеку в IP,
    //далі програма продовжує з наступної інструкції після func()

    //ПРОБЛЕМА: Якщо на вершині стека IP, то нема місця для адреси повернення, через що при завершенні func()
    //процесор не знатиме, куди повернутися і скоріш за все звернеться до несправжньої адреси або зробить щось непотрібне
    //виклик func() призведе до збою програми, бо процесор забуде що робити після func()
    func();

    printf("After func\n");
}
