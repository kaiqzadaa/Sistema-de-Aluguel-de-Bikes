# 🚲 Sistema de Aluguel de Bikes – SA-MP [open.mp]

> Sistema avançado de aluguel de bikes com pickup interativo, mensagem flutuante e integração com MySQL.  
> Desenvolvido com carinho por [@kaiqzadaa](https://github.com/kaiqzadaa) 💙

---

## ✨ Destaques Visuais

・ **Pickup 3D amarelo interativo**  
・ **Texto flutuante** indicando ID e instruções  
・ **Compatível com SA-MP Android & PC**  
・ **Design limpo e roleplay-friendly**

---

## ⚙️ Independências


### YSI-Includes
- Download: [GitHub - pawn-lang/YSI-Includes](https://github.com/pawn-lang/YSI-Includes)

### Pawn.CMD 3.4.0
- Download: [GitHub - katursis/Pawn.CMD](https://github.com/katursis/Pawn.CMD/releases)


---

## 🧱 Prévia no jogo
Veja como fica dentro do servidor:

![Bike Pickup Preview](https://cdn.discordapp.com/attachments/1387605611797024780/1390327816742113331/Screenshot_20250703-104603.jpg?ex=6867db2e&is=686689ae&hm=2a41424fa4a36089946ff4d04fad5351a2b637c50e037686dadf0d9bcae99292&) <!-- Altere essa URL -->

> ✨ Pickup com ícone 3D + texto:  
> `"Itaú Bike\nAperte 'Y' para alugar (ID: 2)"`

---

## 🗃️ Estrutura da Tabela (MySQL)

```sql
CREATE TABLE IF NOT EXISTS `Bikes` (
  `BikeID` INT NOT NULL PRIMARY KEY AUTO_INCREMENT,
  `BikePosX` FLOAT NOT NULL,
  `BikePosY` FLOAT NOT NULL,
  `BikePosZ` FLOAT NOT NULL,
  `BikePosA` FLOAT NOT NULL
);
