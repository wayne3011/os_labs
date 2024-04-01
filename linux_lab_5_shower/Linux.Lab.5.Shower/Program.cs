using Linux.Lab._5.Shower;
using Microsoft.VisualBasic;

List<Person> persons = new()
{
    new Person(Gender.Men, "Паша"),
    new Person(Gender.Men, "Валера"),

    new Person(Gender.Women, "Маша"),
    new Person(Gender.Women, "Лиза"),
    new Person(Gender.Men, "Ваня"),
};
Bathroom bathroom = new Bathroom(5);


foreach (var person in persons)
{
    Thread thread = new Thread(() => bathroom.Enter(person));
    thread.Start();
}